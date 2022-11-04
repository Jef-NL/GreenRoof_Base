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
  // DataPublisher::INSTANCE()->setTransmissionMode(new HTTPTransmission());
  DataPublisher::INSTANCE()->setTransmissionMode(new IOTHubTransmission());
  // Create Bus instance
  sharedBus = new OneWireTemperatureBus(ONE_WIRE_BUS);

  // Add sensors                              |  Sensor name    |     Sensor type           |
  controller->addSensor(new SensorBase::Sensor("test_sen_1", new TestSensor(6)));
  controller->addSensor(new SensorBase::Sensor("test_sen_2", new TestSensor(18)));
  // controller->addSensor(new SensorBase::Sensor("moist_1_green", new MoistureSensor(MOIST1_PIN)));
  // controller->addSensor(new SensorBase::Sensor("moist_2_green", new MoistureSensor(MOIST2_PIN)));
  // controller->addSensor(new SensorBase::Sensor("moist_3_green", new MoistureSensor(MOIST3_PIN)));
  // controller->addSensor(new SensorBase::Sensor("temp_in_green", new DS18B20Sensor(sharedBus, (uint64_t)4035225328881985576)));
  // controller->addSensor(new SensorBase::Sensor("temp_surf_green", new DS18B20Sensor(sharedBus, (uint64_t)504403221035971880)));
  // // controller->addSensor(new SensorBase::Sensor("temp_out_green", new DS18B20Sensor(sharedBus, (uint64_t)0)));
  // controller->addSensor(new SensorBase::Sensor("wr_green", new WaterLevelSensor(WATER_ECHO_PIN, WATER_TRIGGER_PIN)));

  // Run measurements
  controller->runProcess();

  endTimestamp = millis();
  unsigned long timePassed = endTimestamp - startTimestamp;

  // Set controller to sleep
  Serial.printf("Going to sleep for %d milliseconds.\n", ((TIME_TO_SLEEP * 1000) - timePassed));
  Serial.flush();

  esp_sleep_enable_timer_wakeup(((TIME_TO_SLEEP * 1000) - timePassed) * uS_TO_mS_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
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