#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Settings/config.h"

#include "MeasurementController.h"
#include "Sensors/SensorBase.h"
#include "Sensors/TestSensor.h"

// Controller
MeasurementController *controller;

/**************************************************************
 * @brief Single run function on startup
 * 
 **************************************************************/
void setup()
{
  // Start serial communication
  Serial.begin(115200);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Create controller instance
  controller = new MeasurementController();

  // Add sensors
  controller->addSensor(new SensorBase::Sensor("TestSens1", new TestSensor(5)));
  controller->addSensor(new SensorBase::Sensor("TestSens2", new TestSensor(3)));
  controller->addSensor(new SensorBase::Sensor("TestSens3", new TestSensor(4)));

  // Run measurements
  controller->runProcess();

  // Set controller to sleep
  Serial.printf("Going to sleep for %d seconds.\n", TIME_TO_SLEEP);
  Serial.flush();
  esp_deep_sleep_start();
}

/**************************************************************
 * @brief Loop function
 * 
 * Not in use due to deepsleep reset
 **************************************************************/
void loop()
{
}