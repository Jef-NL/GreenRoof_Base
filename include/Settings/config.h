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
#include "Debug.h"

// Sleep time
#define TIME_TO_SLEEP 300 /**< Time ESP32 will go to sleep (in seconds) */

// Select green or non green roof by commenting / uncommenting this
#define GREEN_ROOF true

// Uncomment to publish test data to the database (Device ID TEST)
//#define TESTDATA

// Measurement time settings
#define uS_TO_S_FACTOR 1000000        /**< Conversion factor for micro seconds to seconds */
#define uS_TO_mS_FACTOR 1000          /**< Conversion factor for micro seconds to seconds */
#define ANALOG_MAX_VALUE 4095         /**< Maximum analog measurement value 12 bit resolution */
#define ANALOG_SAMPLE_COUNT 5         /**< Samples per measurement */
#define ANALOG_SAMPLE_DELAY 25        /**< Delay in ms between each measurement */
#define ESP_VOLTAGE_MV 3300           /**< ESP working voltage in millivolts */
#define NETWORK_TIMEOUT 3000          /**< Timeout in ms */
#define TIMESTAMP_NAME "intTimestamp" /**< Time stamp name for database storage */

// Battery
#define BATTERY_LVL_NAME "intBatteryPercentage" /**< Battery percentage name for database storage */
#define BATTERY_ANALOG_PIN 34                   /**< Analog pin for battery measurement on non green roof */
#define BATTERY_R1 360000                       /**< Voltage divider R1 */
#define BATTERY_R2 100000                       /**< Voltage divider R2 */
#define BATTERY_MAX_VOLT 12890                  /**< Max lead-asset battery voltage */
#define BATTERY_MIN_VOLT 11500                  /**< Absolute minimum lead-asset battery voltage*/
#define MIN_BATTERY_LEVEL 40                    /**< Minimum operation battery level in % */
#define BATTERY_SAFETY_ENABLE                   /**< Define if system should enter deepsleep if battery gets below set precentage */

// Temperature sensor DS18B20
#define ONE_WIRE_BUS 4                 /**< Hardware pin address for OneWire bus */
#define MEASUREMENT_VALID_MS 1000      /**< Milliseconds a measurement can be used by the bus connected sensors, to limit polling to often */
#define TEMPERATURE_MULTIPLICATION 100 /**< Multiplication factor for data transmission without decimal points */
#define SENS_TEMP1_NAME "temp_in"      /**< Sensor name */
#define SENS_TEMP2_NAME "temp_surf"    /**< Sensor name */
#define SENS_TEMP3_NAME "temp_out"     /**< Sensor name */

// Moisture sensor SEN0114
#define MOIST1_PIN 34              /**< Analog sensor pin for moisture sensor 1 */
#define MOIST1_VOLT 25             /**< Voltage switch pin for moisture sensor 1 */
#define MOIST2_PIN 35              /**< Analog sensor pin for moisture sensor 2 */
#define MOIST2_VOLT 26             /**< Voltage switch pin for moisture sensor 2 */
#define MOIST3_PIN 32              /**< Analog sensor pin for moisture sensor 3 */
#define MOIST3_VOLT 27             /**< Voltage switch pin for moisture sensor 3 */
#define SENS_MOIST1_NAME "moist_1" /**< Sensor name */
#define SENS_MOIST2_NAME "moist_2" /**< Sensor name */
#define SENS_MOIST3_NAME "moist_3" /**< Sensor name */

// Water level sensor
#define SPEED_OF_SOUND_MS 343.0f       /**< Speed of sound default */
#define WATER_LEVEL_MULTIPLICATION 100 /**< Multiplication factor for data transmission without decimal points */
#define WATER_ECHO_PIN 14              /**< Measurement pin for ultrasonic sensor */
#define WATER_TRIGGER_PIN 12           /**< Trigger pin for ultrasonic sensor */
#define BUCKET_HEIGHT 25               /**< Height in cm of the bucket */
#define BUCKET_TOP_WIDTH 26            /**< Rim width in cm */
#define BUCKET_BOTTOM_WIDTH 22         /**< Bottom width in cm */
#define WATER_SAMPLE_COUNT 5           /**< Samples taken for 1 measurement */
#define SENS_WATER_NAME "water_runoff" /**< Sensor name */

#ifdef GREEN_ROOF
#define MAX_WATER_LVL 26.7 /**< Maximum water level in the bucket */
#else
#define MAX_WATER_LVL 24.88 /**< Maximum water level in the bucket */
#endif

// IOT Hub
#define INCOMING_DATA_BUFFER_SIZE 256
#define IOT_CONFIG_IOTHUB_FQDN "green-roof-prototype.azure-devices.net"
#ifdef GREEN_ROOF
#define IOT_CONFIG_DEVICE_ID "ESP-Green-Roof"
#define IOT_CONFIG_DEVICE_KEY "9rEsrXkzDrz6YvsTh9SKhekWhFC+1KbIHLOZZB2gI+o="
#else
#define IOT_CONFIG_DEVICE_ID "ESP-Normal-Roof"
#define IOT_CONFIG_DEVICE_KEY "U9uLaYK2PDGLKuzG74k4v+hEKhcqDQtSm2dD/L46iIk="
#endif
#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)"
#define SAS_TOKEN_DURATION_IN_MINUTES 60
#define MQTT_QOS1 1
#define DO_NOT_RETAIN_MSG 0

// Webserver
#define WEBSERVER_URL "http://192.168.0.175/SensorPub.php" /**< Webserver URL base for data transport to database */
#define POST_CONTEXT_TYPE "application/json"               /**< Data contect type for POST */
#define JSON_POST_DOC_SIZE 1024                            /**< Size of DynamicJsonDocument */

// Data Storage
#define STORAGE_FILE "/local_data.bin" /**< Local storage file name */