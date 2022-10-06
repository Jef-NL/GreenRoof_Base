/**
 * @file MoistureSensor.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Moisture sensor class
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>

#include "Settings/config.h"
#include "SensorBase.h"

/**
 * @brief Analog moisture sensor implementation
 *
 **/
class MoistureSensor : public SensorBase
{
public:
    /**
     * @brief Construct a new Moisture Sensor object
     *
     * @param pin Analog pin
     **/
    MoistureSensor(uint8_t pin);

    /**
     * @brief Destroy the Moisture Sensor object
     *
     **/
    virtual ~MoistureSensor();

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
     * @return uint16_t Moisture level in promille (1/1000th)
     **/
    uint16_t getMeasurementResult() override;

private:
    uint8_t _sensorPin; /**< Analog sensor pin */
};