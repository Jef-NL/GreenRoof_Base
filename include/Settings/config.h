/**
 * @file config.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief System configuration
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include "keys.h"

// Measurement time settings
#define uS_TO_S_FACTOR 1000000                             /**< Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 10                                   /**< Time ESP32 will go to sleep (in seconds) */
#define WEBSERVER_URL "http://192.168.0.175/SensorPub.php" /**< Webserver URL base for data transport to database */
#define POST_CONTEXT_TYPE "application/json"               /**< Data contect type for POST */
#define JSON_POST_DOC_SIZE 1024                            /**< Size of DynamicJsonDocument */

// Temperature sensor DS18B20
#define ONE_WIRE_BUS 4                 /**< Hardware pin address for OneWire bus */
#define MEASUREMENT_VALID_MS 1000      /**< Milliseconds a measurement can be used by the bus connected sensors, to limit polling to often */
#define TEMPERATURE_MULTIPLICATION 100 /**< Multiplication factor for data transmission without decimal points */
// Sens Address: 4035225328881985576 Address: 504403221035971880

// Moisture sensor SEN0114
#define ANALOG_SAMPLE_COUNT 5 /**< Samples per measurement */
#define ANALOG_MAX_VALUE 4095 /**< Maximum analog measurement value 12 bit resolution */
#define MOIST1_PIN 34         /**< Analog sensor pin for moisture sensor 1 */