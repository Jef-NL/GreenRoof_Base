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
#define uS_TO_S_FACTOR 1000000           /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 10                 /* Time ESP32 will go to sleep (in seconds) */
#define WEBSERVER_URL "https://httpbin.org/post" /**< Webserver URL base for data transport to database */
#define POST_CONTEXT_TYPE "text/plain"   /**< Data contect type for POST */
#define JSON_POST_DOC_SIZE 1024          /**< Size of DynamicJsonDocument */

// Temperature sensor DS18B20
#define ONE_WIRE_BUS 4
#define MEASUREMENT_VALID_MS 1000
#define TEMPERATURE_MULTIPLICATION 100

//Sens Address: 4035225328881985576 Address: 504403221035971880