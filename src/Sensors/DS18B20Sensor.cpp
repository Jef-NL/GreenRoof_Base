/**
 * @file DS18B20Sensor.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Temperature sensor class
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Sensors/DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(OneWireTemperatureBus *sharedBus) : _sharedBus(sharedBus), _busAddress(UINT64_MAX)
{
}

DS18B20Sensor::DS18B20Sensor(OneWireTemperatureBus *sharedBus, uint64_t address) : _sharedBus(sharedBus), _busAddress(address)
{
}

DS18B20Sensor::~DS18B20Sensor()
{
}

bool DS18B20Sensor::startMeasurement()
{
    float measurement = 0.0f;
    if (_busAddress == UINT64_MAX)
    {
        measurement = _sharedBus->getTemperatureForIndex(0);
    }
    else
    {
        measurement = _sharedBus->getTemperatureForAddress(_busAddress);
    }

    if (measurement == SENSOR_INVALID)
    {
        DEBUG_WARN("Measurement Invalid\n");
        return false;
    }

    _sensorValue = (int16_t)roundf(measurement * TEMPERATURE_MULTIPLICATION);
    return true;
}

int16_t DS18B20Sensor::getMeasurementResult()
{
    return _sensorValue;
}