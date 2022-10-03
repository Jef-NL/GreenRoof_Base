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
  controller->addSensor(new SensorBase::Sensor("TestSens", new TestSensor()));

  // Run measurements
  controller->runProcess();

  // Set controller to sleep
  Serial.printf("Going to sleep for %d seconds.\n", TIME_TO_SLEEP);
  Serial.flush();
  esp_deep_sleep_start();
}

void loop()
{

  // if (WiFi.status() == WL_CONNECTED)
  // { 
  //   // Check WiFi connection status
  //   HTTPClient http;

  //   http.begin("http://jsonplaceholder.typicode.com/posts"); // Specify destination for HTTP request
  //   http.addHeader("Content-Type", "text/plain");            // Specify content-type header

  //   int httpResponseCode = http.POST("POSTING from ESP32"); // Send the actual POST request

  //   if (httpResponseCode > 0)
  //   {
  //     String response = http.getString(); // Get the response to the request

  //     Serial.println(httpResponseCode); // Print return code
  //     Serial.println(response);         // Print request answer
  //   }
  //   else
  //   {

  //     Serial.print("Error on sending POST: ");
  //     Serial.println(httpResponseCode);
  //   }

  //   http.end(); // Free resources
  // }
  // else
  // {
  //   Serial.println("Error in WiFi connection");
  // }

  // delay(10000); // Send a request every 10 seconds
}