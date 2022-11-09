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
#define uS_TO_mS_FACTOR 1000                               /**< Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 30                                   /**< Time ESP32 will go to sleep (in seconds) */
#define WEBSERVER_URL "http://192.168.0.175/SensorPub.php" /**< Webserver URL base for data transport to database */
#define POST_CONTEXT_TYPE "application/json"               /**< Data contect type for POST */
#define JSON_POST_DOC_SIZE 1024                            /**< Size of DynamicJsonDocument */
#define TIMESTAMP_NAME "intTimestamp"                      /**< Time stamp name for database storage */

// Temperature sensor DS18B20
#define ONE_WIRE_BUS 4                    /**< Hardware pin address for OneWire bus */
#define MEASUREMENT_VALID_MS 1000         /**< Milliseconds a measurement can be used by the bus connected sensors, to limit polling to often */
#define TEMPERATURE_MULTIPLICATION 100    /**< Multiplication factor for data transmission without decimal points */
#define SENS_TEMP1_NAME "temp_in_green"   /**< Sensor name */
#define SENS_TEMP2_NAME "temp_surf_green" /**< Sensor name */
#define SENS_TEMP3_NAME "temp_out_green"  /**< Sensor name */
// Sens Address: 4035225328881985576 Address: 504403221035971880

// Moisture sensor SEN0114
#define ANALOG_SAMPLE_COUNT 5            /**< Samples per measurement */
#define ANALOG_SAMPLE_DELAY 25           /**< Delay in ms between each measurement */
#define ANALOG_MAX_VALUE 4095            /**< Maximum analog measurement value 12 bit resolution */
#define MOIST1_PIN 34                    /**< Analog sensor pin for moisture sensor 1 */
#define MOIST1_VOLT 25                   /**< Voltage switch pin for moisture sensor 1 */
#define MOIST2_PIN 35                    /**< Analog sensor pin for moisture sensor 2 */
#define MOIST2_VOLT 26                   /**< Voltage switch pin for moisture sensor 2 */
#define MOIST3_PIN 32                    /**< Analog sensor pin for moisture sensor 3 */
#define MOIST3_VOLT 27                   /**< Voltage switch pin for moisture sensor 3 */
#define SENS_MOIST1_NAME "moist_1_green" /**< Sensor name */
#define SENS_MOIST2_NAME "moist_2_green" /**< Sensor name */
#define SENS_MOIST3_NAME "moist_3_green" /**< Sensor name */

// Water level sensor
#define SPEED_OF_SOUND_MS 343.0f       /**< Speed of sound default */
#define WATER_LEVEL_MULTIPLICATION 100 /**< Multiplication factor for data transmission without decimal points */
#define WATER_ECHO_PIN 14              /**< Measurement pin for ultrasonic sensor */
#define WATER_TRIGGER_PIN 12           /**< Trigger pin for ultrasonic sensor */
#define SENS_WATER_NAME "wr_green"     /**< Sensor name */

// IOT Hub
#define INCOMING_DATA_BUFFER_SIZE 256
#define IOT_CONFIG_IOTHUB_FQDN "green-roof-prototype.azure-devices.net"
#define IOT_CONFIG_DEVICE_ID "TestESP"
#define IOT_CONFIG_DEVICE_KEY "4LBZcoSebA7xvzPg3vIpSnIieqU2lJBv5IeyenYgyX4="
#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)"
#define SAS_TOKEN_DURATION_IN_MINUTES 60
#define MQTT_QOS1 1
#define DO_NOT_RETAIN_MSG 0

// Data Storage
#define STORAGE_FILE "/local_data.bin" /**< Local storage file name */