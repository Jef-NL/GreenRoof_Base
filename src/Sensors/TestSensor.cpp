/**
 * @file TestSensor.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Test sensor class
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Sensors/TestSensor.h"

TestSensor::TestSensor(int testVal) : _localVal(testVal)
{
}

TestSensor::~TestSensor()
{
}

bool TestSensor::startMeasurement()
{
    this->_sensorValue = this->_localVal;
    return true;
}

int16_t TestSensor::getMeasurementResult()
{
    return (int16_t)this->_sensorValue;
}
