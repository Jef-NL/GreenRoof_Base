/**
 * @file MoistureSensor.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Moisture sensor class
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Sensors/MoistureSensor.h"

MoistureSensor::MoistureSensor(uint8_t pin, uint8_t voltPin) : _sensorPin(pin), _voltagePin(voltPin)
{
    pinMode(voltPin, OUTPUT);
    digitalWrite(voltPin, HIGH);
}

MoistureSensor::~MoistureSensor()
{
    digitalWrite(_voltagePin, LOW); // Auto shutsdown on sleep.
}

bool MoistureSensor::startMeasurement()
{
    if (!adcAttachPin(_sensorPin))
        return false;

    int measurementSum = 0;

    for (int i = 0; i < ANALOG_SAMPLE_COUNT; i++)
    {
        uint16_t reading = analogRead(_sensorPin);
        measurementSum += (int)reading;
        delay(ANALOG_SAMPLE_DELAY);
    }

    _sensorValue = (uint16_t)((measurementSum / ANALOG_SAMPLE_COUNT) / (ANALOG_MAX_VALUE / 1000));
    return true;
}

uint16_t MoistureSensor::getMeasurementResult()
{
    return _sensorValue;
}
