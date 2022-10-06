/**
 * @file DataPublisher.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Data parser and management for transmission and storage of sensor data
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "DataPublisher.h"

DataPublisher::DataPublisher() : _connected(false), _disconnected(false), _parsedData(""), _lastTimestamp(0L)
{
}

DataPublisher::~DataPublisher()
{
    if (_rawData != nullptr)
        delete _rawData;
}

DataPublisher *DataPublisher::INSTANCE()
{
    static DataPublisher instance;
    return &instance;
}

void DataPublisher::wirelessConnect()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connecting to: %s\n", WIFI_SSID);
    WiFi.onEvent(DataPublisher::wirelessEvent);
}

bool DataPublisher::wirelessGetConnected()
{
    return _connected;
}

void DataPublisher::wirelessDisconnect()
{
    WiFi.disconnect();
    _connected = false;
    _disconnected = true;
}

void DataPublisher::wirelessEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_WIFI_READY:
        Serial.println("WiFi interface ready");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("Connected to WiFi network");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        DataPublisher::INSTANCE()->_connected = true;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    case SYSTEM_EVENT_STA_LOST_IP:
        Serial.println("Disconnected from WiFi network");
        DataPublisher::INSTANCE()->_connected = false;
        DataPublisher::INSTANCE()->_disconnected = true;
        break;
    default:
        Serial.printf("[WiFi-event] Unhandeled event: %d\n", event);
        break;
    }
}

void DataPublisher::setData(DataObject data)
{
    _rawData = &data;
}

void DataPublisher::addData(String fieldName, int fieldValue)
{
    if (_rawData == nullptr)
    {
        _rawData = new DataObject();
    }

    _rawData->items.push_back(new DataEntry(fieldName, fieldValue));
    Serial.printf("Stored field: %s Data: %d\n", fieldName.c_str(), fieldValue);
}

bool DataPublisher::sendData()
{
    if (_rawData == nullptr)
        return false;

    // Wait for connection while not disconnected
    while (!this->wirelessGetConnected() && !this->_disconnected)
        delayMicroseconds(10);

    // Update timestamp for measurement
    this->updateTimestamp();

    if (this->_disconnected)
    {
        Serial.println("Connecting failed, Got disconnected...");
        // ToDo: Store data in memory
        return false;
    }

    // Send HTTP post
    // this->sendHTTPPost();

    // Clear data after it has been sended
    delete _rawData;
    return true;
}

bool DataPublisher::sendHTTPPost()
{
    _httpClient.begin(String(WEBSERVER_URL));
    _httpClient.addHeader("Content-Type", String(POST_CONTEXT_TYPE));

    String content = this->parseData();
    int response = _httpClient.POST(content);
    _httpClient.end();

    switch (response)
    {
    case HTTP_CODE_OK:
        Serial.printf("[HTTP] Success %d\n", response);
        return true;
        break;
    case HTTP_CODE_UNPROCESSABLE_ENTITY:
        Serial.printf("[HTTP] Unprocessable Entity. Data contains non integer type. %s\n", content);
        break;
    case HTTP_CODE_METHOD_NOT_ALLOWED:
        Serial.printf("[HTTP] Method not allowed. %d\n", response);
        break;
    case HTTP_CODE_BAD_REQUEST:
        Serial.printf("[HTTP] Content type or payload not correctly formatted. %d\n", response);
        break;
    case HTTP_CODE_SERVICE_UNAVAILABLE:
        Serial.printf("[HTTP] Service Unavalible. Database might be offline. %d\n", response);
        break;
    default:
        Serial.printf("Unhandled HTTP response: %d\n", response);
        break;
    }

    return false;
}

String DataPublisher::parseData()
{
    DynamicJsonDocument doc(JSON_POST_DOC_SIZE);

    // Add timestamp
    doc[String("intTimestamp")] = this->_lastTimestamp;

    // Add sensor data
    for (auto entry : this->_rawData->items)
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

time_t DataPublisher::updateTimestamp()
{
    // Configure NTP
    configTime(0, 0, NTP_SERVER);
    setenv("TZ", TZ_INFO, 1);

    // Get current time
    tm time;
    getLocalTime(&time);
    this->showTime(time);

    _lastTimestamp = mktime(&time);
    return _lastTimestamp;
}

void DataPublisher::showTime(tm localTime)
{
    Serial.printf("%02d-%02d-%04d %02d:%02d:%02d\n",
                  localTime.tm_mday,
                  localTime.tm_mon + 1,
                  localTime.tm_year + 1900,
                  localTime.tm_hour,
                  localTime.tm_min,
                  localTime.tm_sec);
}
