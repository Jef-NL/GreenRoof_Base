/**
 * @file main.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Main runnable
 * @version 0.1
 * @date 2022-10-05
 *
 * @copyright Copyright (c) 2022
 *
 **/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_sleep.h>
#include <HTTPClient.h>
#include "Settings/config.h"
#include "Transmit/HTTPTransmission.h"
#include "Transmit/IOTHubTransmission.h"

#include "MeasurementController.h"
#include "Sensors/Com/OneWireTemperatureBus.h"
#include "Sensors/SensorBase.h"
#include "Sensors/TestSensor.h"
#include "Sensors/DS18B20Sensor.h"
#include "Sensors/MoistureSensor.h"
#include "Sensors/WaterLevelSensor.h"

// Controller
MeasurementController *controller;

// One Wire shared temperature bus
OneWireTemperatureBus *sharedBus;

// Timing
unsigned long startTimestamp = 0L;
unsigned long endTimestamp = 0L;

/**
 * @brief Single run function on startup
 *
 **/
void setup()
{
  startTimestamp = millis();
  // Start serial communication
  Serial.begin(115200);

  // Create controller instance
  controller = new MeasurementController();

  // Set transmission type
  // DataPublisher::INSTANCE()->setTransmissionMode(new HTTPTransmission());
  DataPublisher::INSTANCE()->setTransmissionMode(new IOTHubTransmission());

  // Create Bus instance
  sharedBus = new OneWireTemperatureBus(ONE_WIRE_BUS);

  // Add sensors                              |  Sensor name    |     Sensor type           |
  // controller->addSensor(new SensorBase::Sensor(SENS_TEMP1_NAME, new TestSensor(6)));
  // controller->addSensor(new SensorBase::Sensor(SENS_TEMP2_NAME, new TestSensor(18)));
  //controller->addSensor(new SensorBase::Sensor(SENS_WATER_NAME, new WaterLevelSensor(WATER_ECHO_PIN, WATER_TRIGGER_PIN)));
#ifdef GREEN_ROOF
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP1_NAME, new DS18B20Sensor(sharedBus, (uint64_t)2594073448133232936)));  //(uint64_t)4035225328881985576))); // Green - Non Green
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP2_NAME, new DS18B20Sensor(sharedBus, (uint64_t)10160120822101062952))); //(uint64_t)16645304285521504040)));
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP3_NAME, new DS18B20Sensor(sharedBus, (uint64_t)15132094810723986984))); //(uint64_t)504403221035971880)));
  controller->addSensor(new SensorBase::Sensor(SENS_MOIST1_NAME, new MoistureSensor(MOIST1_PIN, MOIST1_VOLT)));
  controller->addSensor(new SensorBase::Sensor(SENS_MOIST2_NAME, new MoistureSensor(MOIST2_PIN, MOIST2_VOLT)));
  controller->addSensor(new SensorBase::Sensor(SENS_MOIST3_NAME, new MoistureSensor(MOIST3_PIN, MOIST3_VOLT)));
#else
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP1_NAME, new DS18B20Sensor(sharedBus, (uint64_t)4035225328881985576)));
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP2_NAME, new DS18B20Sensor(sharedBus, (uint64_t)16645304285521504040)));
  controller->addSensor(new SensorBase::Sensor(SENS_TEMP3_NAME, new DS18B20Sensor(sharedBus, (uint64_t)504403221035971880)));
#endif

  // Run measurements
  controller->runProcess();

  endTimestamp = millis();
  unsigned long timePassed = endTimestamp - startTimestamp;

  // Set controller to sleep
  btStop();
  esp_bt_controller_disable();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();

  Serial.printf("Going to sleep for %lu milliseconds.\n", ((TIME_TO_SLEEP * 1000) - timePassed));
  Serial.flush();

  esp_sleep_enable_timer_wakeup(((TIME_TO_SLEEP * 1000) - timePassed) * uS_TO_mS_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
}

/**
 * @brief Loop function
 *
 * Not in use due to deepsleep reset
 **/
void loop()
{
}