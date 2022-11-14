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

RTC_DATA_ATTR int32_t channel = -1;

DataPublisher::DataPublisher() : _connected(false), _disconnected(false), _lastTimestamp(0L)
{
    _dataStorage = new DataStore();
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
    WiFi.mode(WIFI_STA);
    if (channel > -1)
    {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD, channel);
        Serial.println("PRESET CHANNEL");
    }
    else
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
        Serial.printf("Channel %d\n", WiFi.channel());
        channel = WiFi.channel();
        DataPublisher::INSTANCE()->_connected = true;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    case SYSTEM_EVENT_STA_LOST_IP:
        Serial.println("Disconnected from WiFi network");
        DataPublisher::INSTANCE()->_connected = false;
        DataPublisher::INSTANCE()->_disconnected = true;
        break;
    case SYSTEM_EVENT_STA_STOP:
        Serial.println("Connection closed");
        break;
    default:
        Serial.printf("[WiFi-event] Unhandeled event: %d\n", event);
        break;
    }
}

void DataPublisher::setTransmissionMode(TransmissionBase *instance)
{
    if (instance != nullptr)
        _dataEndpoint = instance;
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
    if (_rawData == nullptr || _dataEndpoint == nullptr)
        return false;

    // Wait for connection while not disconnected
    while (!this->wirelessGetConnected() && !this->_disconnected)
        delayMicroseconds(10);

    // Update timestamp for measurement
    _rawData->timestamp = this->updateTimestamp();
#ifndef GREEN_ROOF
    _rawData->batteryLevel = this->readBattery();
#endif

    if (this->_disconnected)
    {
        Serial.println("Connecting failed, Got disconnected...");
        channel = -1;
        _dataStorage->storeDataObject(*_rawData);
        return false;
    }

    // Send data away
    if (!_dataEndpoint->transmitData(_rawData))
    {
        Serial.println("Data failed to send...");
        _dataStorage->storeDataObject(*_rawData);
        return false;
    }

    // Data has been send sucessfully before
    if (_dataStorage->dataAvailable())
    {
        _dataStorage->transmitDataStorage(_dataEndpoint);
    }

    // Clear data after it has been sended
    delete _rawData;
    _dataEndpoint->close();
    return true;
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

float DataPublisher::map(float x, float in_min, float in_max, float out_min, float out_max)
{
    float remappedValue = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    remappedValue = remappedValue > out_max ? out_max : remappedValue;
    remappedValue = remappedValue < out_min ? out_min : remappedValue;
    return remappedValue;
}

#ifndef GREEN_ROOF
int16_t DataPublisher::readBattery()
{
    float factor = 1.0f / ((float)BATTERY_R2 / ((float)BATTERY_R1 + (float)BATTERY_R2));
    float batteryConstant = factor * ((float)ESP_VOLTAGE_MV / (float)ANALOG_MAX_VALUE);

    int measurementSum = 0;
    for (int i = 0; i < ANALOG_SAMPLE_COUNT; i++)
    {
        uint16_t reading = analogRead(BATTERY_ANALOG_PIN);
        measurementSum += (int)reading;
        delay(ANALOG_SAMPLE_DELAY);
    }

    int analogRead = (uint16_t)(measurementSum / ANALOG_SAMPLE_COUNT);
    float batteryVoltage = (float)analogRead * batteryConstant;

    return (int16_t)this->map(batteryVoltage, BATTERY_MIN_VOLT, BATTERY_MAX_VOLT, 0, 100);
}
#endif
