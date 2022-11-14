/**
 * @file WaterLevelSensor.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Water level sensor class
 * @version 0.1
 * @date 2022-10-13
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Sensors/WaterLevelSensor.h"

WaterLevelSensor::WaterLevelSensor(uint8_t echoPin, uint8_t triggerPin) : _echoPin(echoPin), _triggerPin(triggerPin)
{
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);
}

WaterLevelSensor::~WaterLevelSensor()
{
}

bool WaterLevelSensor::startMeasurement()
{
    // Init pulse
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

    // Measure duration
    float duration = pulseIn(_echoPin, HIGH); // in microseconds

    if (duration < 0.1f) // Value invalid
        return false;

    float distance = duration * ((float)SPEED_OF_SOUND_MS / 10000.0f) / 2.0f; // in cm

    float waterMl = calculateWaterLevel(distance * 10.0);
    _sensorValue = (uint16_t)(waterMl * (float)WATER_LEVEL_MULTIPLICATION);

    return true;
}

uint16_t WaterLevelSensor::getMeasurementResult()
{
    return _sensorValue;
}

float WaterLevelSensor::calculateWaterLevel(float mm)
{
    float heightInCm = mm / 10.0;
    float topDiam = BUCKET_TOP_WIDTH / 2.0;
    float bottomDiam = BUCKET_BOTTOM_WIDTH / 2.0;
    float bucketTaper = (topDiam - bottomDiam) / BUCKET_HEIGHT;

    float rimSize = bottomDiam + (heightInCm * bucketTaper);
    float waterLvlMillitre = (1.0 / 3.0 * PI) * heightInCm * (pow(rimSize, 2.0) + (rimSize * bottomDiam) + pow(bottomDiam, 2.0));
   
    return waterLvlMillitre;
}
