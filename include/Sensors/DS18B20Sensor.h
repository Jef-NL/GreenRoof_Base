/**
 * @file Ds18B20Sensor.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Temperature sensor class
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>

#include "SensorBase.h"
#include "Sensors/Com/OneWireTemperatureBus.h"

/**
 * @brief One wire temperature sensor implementation
 *
 **/
class DS18B20Sensor : public SensorBase
{
public:
    /**
     * @brief Construct a new DS18B20Sensor object
     *
     * Constructor for use with single sensor only.
     * @param sharedBus Shared OneWire bus for multiple sensors
     **/
    DS18B20Sensor(OneWireTemperatureBus *sharedBus);

    /**
     * @brief Construct a new DS18B20Sensor object
     *
     * @param sharedBus Shared OneWire bus for multiple sensors
     * @param address OneWire 64 bit bus address
     **/
    DS18B20Sensor(OneWireTemperatureBus *sharedBus, uint64_t address);

    /**
     * @brief Destroy the DS18B20 Sensor object
     *
     **/
    virtual ~DS18B20Sensor();

public:
    /**
     * @brief Run sensor measurement steps
     *
     * @return true Measurement successful
     * @return false Measurement failed
     **/
    bool startMeasurement() override;

    /**
     * @brief Get the measurement results
     *
     * @return int16_t Temperature in degrees * TEMPERATURE_MULTIPLICATION
     **/
    int16_t getMeasurementResult() override;

private:
    OneWireTemperatureBus *_sharedBus; /**< Shared OneWire bus for communication */
    uint64_t _busAddress;              /**< 64 bit unique bus identifier */
};
