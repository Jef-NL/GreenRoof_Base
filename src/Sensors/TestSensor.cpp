/**************************************************************
 * @file TestSensor.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **************************************************************/
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

int TestSensor::getMeasurementResult()
{
    return this->_sensorValue;
}
