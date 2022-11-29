/**
 * @file TestSensor.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Test sensor class
 * @version 0.1
 * @date 2022-10-03
 * 
 * @copyright Copyright (c) 2022
 * 
 **/
#pragma once
#include "SensorBase.h"

class TestSensor : public SensorBase
{
public:
    /**
     * @brief Construct a new Test Sensor object
     * 
     **/
    TestSensor(int testVal);

    /**
     * @brief Destroy the Test Sensor object
     * 
     **/
    virtual ~TestSensor();

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
     * @return uint16_t Sensor value
     **/
    int16_t getMeasurementResult() override;

private:
    int16_t _localVal;  /**< Local test value */
};

