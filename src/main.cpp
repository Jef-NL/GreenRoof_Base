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

/**
 * @brief Single run function on startup
 *
 **/
void setup()
{
  // Start serial communication
  Serial.begin(115200);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Create controller instance
  controller = new MeasurementController();
  // Create Bus instance
  sharedBus = new OneWireTemperatureBus(ONE_WIRE_BUS);

  // Add sensors                              |  Sensor name    |     Sensor type           |
  controller->addSensor(new SensorBase::Sensor("moist_1_green", new MoistureSensor(MOIST1_PIN)));
  controller->addSensor(new SensorBase::Sensor("moist_2_green", new MoistureSensor(MOIST2_PIN)));
  controller->addSensor(new SensorBase::Sensor("moist_3_green", new MoistureSensor(MOIST3_PIN)));
  controller->addSensor(new SensorBase::Sensor("temp_in_green", new DS18B20Sensor(sharedBus, (uint64_t)4035225328881985576)));
  controller->addSensor(new SensorBase::Sensor("temp_surf_green", new DS18B20Sensor(sharedBus, (uint64_t)504403221035971880)));
  // controller->addSensor(new SensorBase::Sensor("temp_out_green", new DS18B20Sensor(sharedBus, (uint64_t)0)));
  controller->addSensor(new SensorBase::Sensor("wr_green", new WaterLevelSensor(WATER_ECHO_PIN, WATER_TRIGGER_PIN)));

  // Run measurements
  controller->runProcess();

  // Set controller to sleep
  Serial.printf("Going to sleep for %d seconds.\n", TIME_TO_SLEEP);
  Serial.flush();
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