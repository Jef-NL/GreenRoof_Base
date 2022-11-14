/**
 * @file WaterLevelSensor.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Water level sensor class
 * @version 0.1
 * @date 2022-10-13
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>
#include <math.h>

#include "SensorBase.h"

/**
 * @brief Ultrasonic distance sensor class for calculating water level.
 *
 **/
class WaterLevelSensor : public SensorBase
{
public:
    /**
     * @brief Construct a new Water Level Sensor object
     *
     * @param echoPin Echo measurement pin
     * @param triggerPin Trigger setup pin
     **/
    WaterLevelSensor(uint8_t echoPin, uint8_t triggerPin);

    /**
     * @brief Destroy the Water Level Sensor object
     *
     **/
    virtual ~WaterLevelSensor();

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
     * @return uint16_t Water level in ml
     **/
    uint16_t getMeasurementResult() override;

private:
    float calculateWaterLevel(float mm);

    const uint8_t _echoPin;    /**< Echo pin */
    const uint8_t _triggerPin; /**< Trigger pin */
};
