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

TestSensor::TestSensor()
{
}

TestSensor::~TestSensor()
{
}

bool TestSensor::startMeasurement()
{
    this->_sensorValue = int(5);
    return true;
}

int TestSensor::getMeasurementResult()
{
    return this->_sensorValue;
}
