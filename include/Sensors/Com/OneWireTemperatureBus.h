/**************************************************************
 * @file OneWireTemperatureBus.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Class for shared bus instance of temperature bus
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 **************************************************************/
#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Settings/config.h"

#define SENSOR_INVALID -127.0

/**************************************************************
 * @brief Shared OneWire temperature bus object
 *
 * An instance of this class will be used by the DS18B20Sensor class
 **************************************************************/
class OneWireTemperatureBus
{
public:
    /**************************************************************
     * @brief Construct a new One Wire Temperature Bus object
     *
     * @param busPin Hardware Pin Address
     **************************************************************/
    OneWireTemperatureBus(uint8_t busPin);

    /**************************************************************
     * @brief Destroy the One Wire Temperature Bus object
     *
     **************************************************************/
    virtual ~OneWireTemperatureBus();

public:
    /**************************************************************
     * @brief Get the Temperature For Index object
     *
     * @param index
     * @return float
     **************************************************************/
    float getTemperatureForIndex(uint8_t index);

    /**************************************************************
     * @brief Get the Temperature for the sensor with Address
     *
     * @param address Uint 8 array
     * @return float Temperature in degrees Celsius
     **************************************************************/
    float getTemperatureForAddress(uint8_t *address);

    /**************************************************************
     * @brief Get the Temperature for the sensor with Address
     *
     * @param address Uint 64 value
     * @return float Temperature in degrees Celsius
     **************************************************************/
    float getTemperatureForAddress(uint64_t address);

private:
    /**************************************************************
     * @brief Run and buffer measurements for all connected sensors
     *
     **************************************************************/
    void runMeasurements();

    /**************************************************************
     * @brief Address translation helper function
     *
     * @param address Uint 8 address
     * @return uint64_t Uint 64 address
     **************************************************************/
    uint64_t addressArrayToUint64(uint8_t *address);

    /**************************************************************
     * @brief Address translation helper function
     *
     * @param outpAddress Output address array
     * @param address Uint 64 address
     **************************************************************/
    void addressUint64ToArray(uint8_t *outpAddress, uint64_t address);

    OneWire *_oneWireBus;                 /*< OneWire bus instance */
    DallasTemperature *_connectedSensors; /*< DallasTemperature instance for DS18B20 sensors */
    int _deviceCount;                     /*< Amount of discovered bus devices*/
    unsigned long _lastMeasurementStamp;  /*< Last millisecond timestamp of measurement */
};
