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

    _sonar = new NewPing(triggerPin, echoPin, 30U);
}

WaterLevelSensor::~WaterLevelSensor()
{
}

bool WaterLevelSensor::startMeasurement()
{
    //! Deprecated
    // float distance = 0.0f;

    // for (int i = 0; i < WATER_SAMPLE_COUNT ; i++)
    // {
    //     distance += this->measureDistance();
    //     delay(10);
    // }
    // distance = distance / (float)WATER_SAMPLE_COUNT;

    float duration = _sonar->ping_median(20); // 10 interations - returns duration in microseconds
    duration = duration / 1000000;            // Convert mircroseconds to seconds
    float distance = (SPEED_OF_SOUND_MS * duration) / 2;
    distance = distance * 100; // meters to centimeters

    if (distance < 0)
        return false;

    MEASURE_LOG("[CALIBRATE] Dist: %f\n", distance);

    float waterMl = calculateWaterLevel(distance);
    waterMl = waterMl < 0 ? 0.0f : waterMl;

    //! Publish mm for now
    _sensorValue = (int16_t)(distance * 100.0f); //(int16_t)(waterMl);

    return true;
}

int16_t WaterLevelSensor::getMeasurementResult()
{
    return _sensorValue;
}

//! Deprecated
float WaterLevelSensor::measureDistance()
{
    // Init pulse
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

    // Measure duration
    noInterrupts();
    unsigned long duration = pulseIn(_echoPin, HIGH); // in microseconds
    interrupts();

    if (duration < 1) // Value invalid
        return -1;

    float distance = (float)duration * ((float)SPEED_OF_SOUND_MS / 10000.0f) / 2.0f; // in cm
    MEASURE_LOG("[CALIBRATE] Dur: %lu Dist: %f \n", duration, distance);

    return distance;
}

float WaterLevelSensor::calculateWaterLevel(float cm)
{
    float heightInCm = (float)MAX_WATER_LVL - cm;
    float topDiam = (float)BUCKET_TOP_WIDTH / 2.0f;
    float bottomDiam = (float)BUCKET_BOTTOM_WIDTH / 2.0f;
    float bucketTaper = (topDiam - bottomDiam) / (float)BUCKET_HEIGHT;

    float rimSize = bottomDiam + (heightInCm * bucketTaper);
    float waterLvlMillitre = (1.0 / 3.0 * PI) * heightInCm * (pow(rimSize, 2.0) + (rimSize * bottomDiam) + pow(bottomDiam, 2.0));

    return waterLvlMillitre;
}
