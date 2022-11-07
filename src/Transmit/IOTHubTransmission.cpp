/**
 * @file IOTHubTransmission.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief IOT Hub based transmission method
 * @version 0.1
 * @date 2022-11-04
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Transmit/IOTHubTransmission.h"
#include <azure_ca.h>

// Statics
static esp_mqtt_client_handle_t mqttClient;
static az_iot_hub_client client;
static char incoming_data[INCOMING_DATA_BUFFER_SIZE];

static const char *host = IOT_CONFIG_IOTHUB_FQDN;
static const char *mqtt_broker_uri = "mqtts://" IOT_CONFIG_IOTHUB_FQDN;
static const char *device_id = IOT_CONFIG_DEVICE_ID;
static const int mqtt_port = AZ_IOT_DEFAULT_MQTT_CONNECT_PORT;
static bool connected = false;

static char mqtt_client_id[128];
static char mqtt_username[128];
static char mqtt_password[200];
static char telemetry_topic[128];
static uint8_t sas_signature_buffer[256];
static AzIoTSasToken sasToken(
    &client,
    AZ_SPAN_FROM_STR(IOT_CONFIG_DEVICE_KEY),
    AZ_SPAN_FROM_BUFFER(sas_signature_buffer),
    AZ_SPAN_FROM_BUFFER(mqtt_password));

IOTHubTransmission::IOTHubTransmission()
{
}

IOTHubTransmission::~IOTHubTransmission()
{
}

bool IOTHubTransmission::transmitData(DataObject *object)
{
    _dataObject = object;
    //? We are connected, so we can setup and send.

    // Setup IOT Hub Client
    initIotHubClient();
    // Setup MQTT Client
    if (sasToken.IsExpired())
    {
        (void)esp_mqtt_client_destroy(mqttClient);
    }
    initMqttClient();

    // Parse Json and convert to char array
    _parsedData = this->parseData();
    size_t length = _parsedData.length();
    char dataArray[length + 1];

    strncpy(dataArray, _parsedData.c_str(), length);
    az_span data = AZ_SPAN_FROM_BUFFER(dataArray);

    Serial.println("Sending telemetry ...");

    // Gather MQTT topic
    if (az_result_failed(az_iot_hub_client_telemetry_get_publish_topic(
            &client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
    {
        Serial.println("Failed az_iot_hub_client_telemetry_get_publish_topic");
        return false;
    }

    if (esp_mqtt_client_publish(
            mqttClient,
            telemetry_topic,
            (const char *)az_span_ptr(data),
            az_span_size(data),
            MQTT_QOS1,
            DO_NOT_RETAIN_MSG) == 0)
    {
        Serial.println("Failed publishing");
        return false;
    }

    uint16_t timeout = 0;
    while (!connected){
        Serial.print(".");
        delay(10);
        timeout++;
        if (timeout > 200) {
            Serial.println("MQTT publish failed");
            return false;
        }
    };
    Serial.println("Published data");

    (void)esp_mqtt_client_destroy(mqttClient);
    return true;
}

void receivedCallback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

esp_err_t IOTHubTransmission::mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
        int i, r;

    case MQTT_EVENT_ERROR:
        Serial.println("MQTT event MQTT_EVENT_ERROR");
        break;
    case MQTT_EVENT_CONNECTED:
        Serial.println("MQTT event MQTT_EVENT_CONNECTED");
        connected = true;
        // r = esp_mqtt_client_subscribe(mqttClient, AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC, 1);
        // if (r == -1)
        // {
        //     Serial.println("Could not subscribe for cloud-to-device messages.");
        // }
        // else
        // {
        //     Serial.println("Subscribed for cloud-to-device messages; message id:" + String(r));
        // }

        break;
    case MQTT_EVENT_DISCONNECTED:
        Serial.println("MQTT event MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        Serial.println("MQTT event MQTT_EVENT_SUBSCRIBED");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        Serial.println("MQTT event MQTT_EVENT_UNSUBSCRIBED");
        break;
    case MQTT_EVENT_PUBLISHED:
        Serial.println("MQTT event MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_DATA:
        Serial.println("MQTT event MQTT_EVENT_DATA");

        for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->topic_len; i++)
        {
            incoming_data[i] = event->topic[i];
        }
        incoming_data[i] = '\0';
        Serial.println("Topic: " + String(incoming_data));

        for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->data_len; i++)
        {
            incoming_data[i] = event->data[i];
        }
        incoming_data[i] = '\0';
        Serial.println("Data: " + String(incoming_data));

        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        Serial.println("MQTT event MQTT_EVENT_BEFORE_CONNECT");
        break;
    default:
        Serial.println("MQTT event UNKNOWN");
        break;
    }

    return ESP_OK;
}

String IOTHubTransmission::parseData()
{
    DynamicJsonDocument doc(JSON_POST_DOC_SIZE);

    // Add timestamp
    doc[String("intTimestamp")] = this->_dataObject->timestamp;

    // Add sensor data
    for (auto entry : this->_dataObject->items)
    {
        // Add measurement
        doc[entry->fieldName.c_str()] = entry->fieldValue;
    }

    // Json to string
    serializeJson(doc, _parsedData);

    // DEBUG
    Serial.println(_parsedData);

    return String(_parsedData);
}

void IOTHubTransmission::initIotHubClient()
{
    az_iot_hub_client_options options = az_iot_hub_client_options_default();
    options.user_agent = AZ_SPAN_FROM_STR(AZURE_SDK_CLIENT_USER_AGENT);

    if (az_result_failed(az_iot_hub_client_init(
            &client,
            az_span_create((uint8_t *)host, strlen(host)),
            az_span_create((uint8_t *)device_id, strlen(device_id)),
            &options)))
    {
        Serial.println("Failed initializing Azure IoT Hub client");
        return;
    }

    size_t client_id_length;
    if (az_result_failed(az_iot_hub_client_get_client_id(
            &client, mqtt_client_id, sizeof(mqtt_client_id) - 1, &client_id_length)))
    {
        Serial.println("Failed getting client id");
        return;
    }

    if (az_result_failed(az_iot_hub_client_get_user_name(
            &client, mqtt_username, sizeofarray(mqtt_username), NULL)))
    {
        Serial.println("Failed to get MQTT clientId, return code");
        return;
    }

    Serial.println("Client ID: " + String(mqtt_client_id));
    Serial.println("Username: " + String(mqtt_username));
}

int IOTHubTransmission::initMqttClient()
{
    if (sasToken.Generate(SAS_TOKEN_DURATION_IN_MINUTES) != 0)
    {
        Serial.println("Failed generating SAS token");
        return 1;
    }

    esp_mqtt_client_config_t mqtt_config;
    memset(&mqtt_config, 0, sizeof(mqtt_config));
    mqtt_config.uri = mqtt_broker_uri;
    mqtt_config.port = mqtt_port;
    mqtt_config.client_id = mqtt_client_id;
    mqtt_config.username = mqtt_username;
    mqtt_config.password = (const char *)az_span_ptr(sasToken.Get());

    mqtt_config.keepalive = 30;
    mqtt_config.disable_clean_session = 0;
    mqtt_config.disable_auto_reconnect = false;
    mqtt_config.event_handle = mqtt_event_handler;
    mqtt_config.user_context = NULL;
    mqtt_config.cert_pem = (const char *)ca_pem;

    mqttClient = esp_mqtt_client_init(&mqtt_config);

    if (mqttClient == NULL)
    {
        Serial.println("Failed creating mqtt client");
        return 1;
    }

    esp_err_t start_result = esp_mqtt_client_start(mqttClient);

    if (start_result != ESP_OK)
    {
        Serial.println("Could not start mqtt client; error code:" + start_result);
        return 1;
    }
    else
    {
        Serial.println("MQTT client started");
        return 0;
    }
}
