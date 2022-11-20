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
static uint16_t pubCount = 0;

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

bool IOTHubTransmission::transmitData(DataObject *object, bool skipSetup)
{
    _dataObject = object;
    //? We are connected, so we can setup and send.

    // Setup IOT Hub Client
    if (!skipSetup)
        initIotHubClient();
    // Setup MQTT Client
    if (sasToken.IsExpired())
    {
        (void)esp_mqtt_client_destroy(mqttClient);
        initMqttClient();
    }
    else
    {
        if (!skipSetup)
            initMqttClient();
    }

    // Parse Json and convert to char array
    _parsedData = this->parseData();
    size_t length = _parsedData.length();
    char dataArray[length + 1];

    strncpy(dataArray, _parsedData.c_str(), length);
    az_span data = AZ_SPAN_FROM_BUFFER(dataArray);

    PUBLISH_LOG("Sending telemetry ...\n");

    // Gather MQTT topic
    if (az_result_failed(az_iot_hub_client_telemetry_get_publish_topic(
            &client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
    {
        PUBLISH_WARN("Failed az_iot_hub_client_telemetry_get_publish_topic\n");
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
        PUBLISH_WARN("Failed publishing\n");
        return false;
    }

    uint16_t timeout = 0;
    while (!connected)
    {
        DEBUG_LOG(".");
        delay(10);
        timeout++;
        if (timeout > 200)
        {
            PUBLISH_WARN("MQTT publish failed\n");
            return false;
        }
    };
    PUBLISH_LOG("Published data\n");
    pubCount++;
    _parsedData = "";
    _dataObject = nullptr;

    return true;
}

void IOTHubTransmission::close()
{
    uint16_t timeout = 0;
    while (pubCount > 0)
    {
        delay(10);
        timeout++;
        if (timeout > (TIME_TO_SLEEP * 100))
        {
            PUBLISH_WARN("MQTT publish Never finished before new measurement\n");
            break;
        }
    }

    (void)esp_mqtt_client_destroy(mqttClient);
}

esp_err_t IOTHubTransmission::mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
        int i, r;

    case MQTT_EVENT_ERROR:
        PUBLISH_WARN("MQTT event MQTT_EVENT_ERROR\n");
        connected = false;
        break;
    case MQTT_EVENT_CONNECTED:
        PUBLISH_LOG("MQTT event MQTT_EVENT_CONNECTED\n");
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
        PUBLISH_LOG("MQTT event MQTT_EVENT_DISCONNECTED\n");
        connected = false;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        PUBLISH_LOG("MQTT event MQTT_EVENT_SUBSCRIBED\n");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        PUBLISH_LOG("MQTT event MQTT_EVENT_UNSUBSCRIBED\n");
        break;
    case MQTT_EVENT_PUBLISHED:
        PUBLISH_LOG("MQTT event MQTT_EVENT_PUBLISHED\n");
        pubCount--;
        break;
    case MQTT_EVENT_DATA:
        PUBLISH_LOG("MQTT event MQTT_EVENT_DATA\n");

        for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->topic_len; i++)
        {
            incoming_data[i] = event->topic[i];
        }
        incoming_data[i] = '\0';
        DEBUG_LOG("Topic: %s\n", String(incoming_data).c_str());

        for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->data_len; i++)
        {
            incoming_data[i] = event->data[i];
        }
        incoming_data[i] = '\0';
        DEBUG_LOG("Data: %s\n", String(incoming_data).c_str());

        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        PUBLISH_LOG("MQTT event MQTT_EVENT_BEFORE_CONNECT\n");
        break;
    default:
        PUBLISH_WARN("MQTT event UNKNOWN\n");
        break;
    }

    return ESP_OK;
}

String IOTHubTransmission::parseData()
{
    DynamicJsonDocument doc(JSON_POST_DOC_SIZE);

    // Add timestamp
    doc[String(TIMESTAMP_NAME)] = this->_dataObject->timestamp;
#ifdef GREEN_ROOF
    doc["deviceId"] = "GREEN";
#else
    doc["deviceId"] = "NORMAL";
    doc[String(BATTERY_LVL_NAME)] = this->_dataObject->batteryLevel;
#endif

    // Add sensor data
    for (auto entry : this->_dataObject->items)
    {
        // Add measurement
        doc[entry->fieldName.c_str()] = entry->fieldValue;
    }

    // Json to string
    serializeJson(doc, _parsedData);

    // DEBUG
    PUBLISH_LOG("%s\n", _parsedData.c_str());

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
        PUBLISH_WARN("Failed initializing Azure IoT Hub client\n");
        return;
    }

    size_t client_id_length;
    if (az_result_failed(az_iot_hub_client_get_client_id(
            &client, mqtt_client_id, sizeof(mqtt_client_id) - 1, &client_id_length)))
    {
        PUBLISH_WARN("Failed getting client id\n");
        return;
    }

    if (az_result_failed(az_iot_hub_client_get_user_name(
            &client, mqtt_username, sizeofarray(mqtt_username), NULL)))
    {
        PUBLISH_WARN("Failed to get MQTT clientId, return code\n");
        return;
    }

    DEBUG_LOG("[MQTT] Client ID: %s\n", String(mqtt_client_id).c_str());
    DEBUG_LOG("[MQTT] Username: %s\n", String(mqtt_username).c_str());
}

int IOTHubTransmission::initMqttClient()
{
    if (sasToken.Generate(SAS_TOKEN_DURATION_IN_MINUTES) != 0)
    {
        PUBLISH_WARN("Failed generating SAS token\n");
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
        PUBLISH_WARN("Failed creating mqtt client\n");
        return 1;
    }

    esp_err_t start_result = esp_mqtt_client_start(mqttClient);

    if (start_result != ESP_OK)
    {
        PUBLISH_WARN("Could not start mqtt client; error code: %d\n", start_result);
        return 1;
    }
    else
    {
        PUBLISH_LOG("MQTT client started\n");
        return 0;
    }
}
