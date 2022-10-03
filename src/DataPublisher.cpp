/**************************************************************
 * @file DataPublisher.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **************************************************************/
#include "DataPublisher.h"

DataPublisher::DataPublisher() : _connected(false), _disconnected(false), _parsedData("")
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

void DataPublisher::addData(std::string fieldName, int fieldValue)
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

    if (this->_disconnected)
    {
        Serial.println("Connecting failed, Got disconnected...");
        return false;
    }

    // ToDo: Parse and send data using HTTP POST

    // Clear data after it has been sended
    delete _rawData;
    return true;
}

std::string DataPublisher::parseData()
{
    // ToDo: Parse data from DataObject to Json string
    return std::string();
}
