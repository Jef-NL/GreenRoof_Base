/**
 * @file SensorBase.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Sensor base object Interface
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>

#include "Settings/config.h"
/**
 * @brief Sensor base class
 *
 **/
class SensorBase
{
public:
    /**
     * @brief Default constructor
     *
     **/
    SensorBase() = default;

    /**
     * @brief Default destructor
     *
     **/
    virtual ~SensorBase() = default;

    /**
     * @brief Sensor registration struct
     *
     **/
    struct Sensor
    {
        /**
         * @brief Construct a new Sensor object
         *
         * @param name Sensor name
         * @param instance Sensor implementation instance
         **/
        Sensor(String name, SensorBase *instance) : sensorName(name), sensor(instance) {}
        String sensorName;  /**< Sensor registration name */
        SensorBase *sensor; /**< Sensor instance */
    };

public:
    /**
     * @brief Run sensor measurement steps
     *
     * Blocking fully virtual function, to be over written.
     * @return true Measurement successful
     * @return false Measurement failed
     **/
    virtual bool startMeasurement() = 0;

    /**
     * @brief Get the measurement results
     *
     * Blocking fully virtual function, to be over written.
     * @return int16_t Sensor value
     **/
    virtual int16_t getMeasurementResult() = 0;

protected:
    int16_t _sensorValue = 0; /**< Sensor value for publishing */
};
