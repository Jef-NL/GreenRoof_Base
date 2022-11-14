/**
 * @file HTTPTransmission.cpp
 * @author Jef (hj.baars@student.avans.nl)
 * @brief Transmission method for HTTP post transfers
 * @version 0.1
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Transmit/HTTPTransmission.h"

HTTPTransmission::HTTPTransmission()
{
}

HTTPTransmission::~HTTPTransmission()
{
}

bool HTTPTransmission::transmitData(DataObject *object, bool skipSetup)
{
    if (object == nullptr)
        return false;

    _dataObject = object;

    _httpClient.begin(String(WEBSERVER_URL));
    _httpClient.addHeader("Content-Type", String(POST_CONTEXT_TYPE));

    String content = this->parseData();
    int response = _httpClient.POST(content);
    _httpClient.end();

    switch (response)
    {
    case HTTP_CODE_OK:
        Serial.printf("[HTTP] Success %d\n", response);
        return true;
        break;
    case HTTP_CODE_UNPROCESSABLE_ENTITY:
        Serial.printf("[HTTP] Unprocessable Entity. Data contains non integer type. %s\n", content.c_str());
        break;
    case HTTP_CODE_METHOD_NOT_ALLOWED:
        Serial.printf("[HTTP] Method not allowed. %d\n", response);
        break;
    case HTTP_CODE_BAD_REQUEST:
        Serial.printf("[HTTP] Content type or payload not correctly formatted. %d\n", response);
        break;
    case HTTP_CODE_SERVICE_UNAVAILABLE:
        Serial.printf("[HTTP] Service Unavalible. Database might be offline. %d\n", response);
        break;
    default:
        Serial.printf("Unhandled HTTP response: %d\n", response);
        break;
    }

    return false;
}

String HTTPTransmission::parseData()
{
    DynamicJsonDocument doc(JSON_POST_DOC_SIZE);

    // Add timestamp
    doc[String(TIMESTAMP_NAME)] = this->_dataObject->timestamp;
#ifndef GREEN_ROOF
    doc[String(BATTERY_LVL_NAME)] = this->_dataObject->batteryLevel;
#endif

    // Add sensor data
    for (auto entry : this->_dataObject->items)
    {
        // Add measurement
        doc[entry->fieldName.c_str()] = entry->fieldValue;
    }

    // Json to string
    serializeJson(doc, _parsedData);

    // DEBUG
    Serial.println(_parsedData);

    return String(_parsedData);
}