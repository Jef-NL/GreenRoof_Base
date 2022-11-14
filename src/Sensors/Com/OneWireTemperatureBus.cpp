/**
 * @file OneWireTemperatureBus.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Class for shared bus instance of temperature bus
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Sensors/Com/OneWireTemperatureBus.h"

OneWireTemperatureBus::OneWireTemperatureBus(uint8_t busPin) : _deviceCount(0), _lastMeasurementStamp(0)
{
    // Init bus
    _oneWireBus = new OneWire(busPin);
    _connectedSensors = new DallasTemperature(_oneWireBus);

    // Start Bus
    _connectedSensors->begin();

    // Find devices
    for (_deviceCount = 0;; _deviceCount++)
    {
        DeviceAddress addr;
        if (!_connectedSensors->getAddress(addr, _deviceCount))
            break;

        Serial.printf("Found Temperature Bus device - Address: %llu \n", this->addressArrayToUint64(addr));
    }
}

OneWireTemperatureBus::~OneWireTemperatureBus()
{
    delete _oneWireBus;
    delete _connectedSensors;
}

float OneWireTemperatureBus::getTemperatureForIndex(uint8_t index)
{
    if (index >= _deviceCount)
        return SENSOR_INVALID;

    // Get sensor data
    this->runMeasurements();

    return _connectedSensors->getTempCByIndex(index);
}

float OneWireTemperatureBus::getTemperatureForAddress(uint8_t *address)
{
    if (address == nullptr || !_connectedSensors->validAddress(address))
        return SENSOR_INVALID;

    // Get sensor data
    this->runMeasurements();

    return _connectedSensors->getTempC(address);
}

float OneWireTemperatureBus::getTemperatureForAddress(uint64_t address)
{
    uint8_t addrArray[sizeof(uint64_t)] = {0};
    this->addressUint64ToArray(addrArray, address);

    if (address == UINT64_MAX || !_connectedSensors->validAddress(addrArray))
        return SENSOR_INVALID;   

    // Get sensor data
    this->runMeasurements();

    return _connectedSensors->getTempC(addrArray);
}

void OneWireTemperatureBus::runMeasurements()
{
    if (millis() > _lastMeasurementStamp + MEASUREMENT_VALID_MS)
        return;

    Serial.println("Running measurement...");
    _lastMeasurementStamp = millis();
    _connectedSensors->requestTemperatures();
}

uint64_t OneWireTemperatureBus::addressArrayToUint64(uint8_t *address)
{
    uint64_t outpAddress;
    memcpy(&outpAddress, address, sizeof(uint64_t));

    return outpAddress;
}

void OneWireTemperatureBus::addressUint64ToArray(uint8_t* outpAddress, uint64_t address)
{
    memcpy(outpAddress, &address, sizeof(uint64_t));
}
