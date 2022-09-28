#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "keys.h"

void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  { //TODO: Move to class
    // Check WiFi connection status
    HTTPClient http;

    http.begin("http://jsonplaceholder.typicode.com/posts"); // Specify destination for HTTP request
    http.addHeader("Content-Type", "text/plain");            // Specify content-type header

    int httpResponseCode = http.POST("POSTING from ESP32"); // Send the actual POST request

    if (httpResponseCode > 0)
    {
      String response = http.getString(); // Get the response to the request

      Serial.println(httpResponseCode); // Print return code
      Serial.println(response);         // Print request answer
    }
    else
    {

      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources
  }
  else
  {
    Serial.println("Error in WiFi connection");
    //TODO: RECONNECT
  }

  delay(10000); // Send a request every 10 seconds
}