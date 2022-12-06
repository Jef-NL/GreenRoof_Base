/**
 * @file UDPTransmission.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Transmission class for publishing to Urban Data Platform
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "Transmit/UDPTransmission.h"

UDPTransmission::UDPTransmission()
{
}

UDPTransmission::~UDPTransmission()
{
}

bool UDPTransmission::transmitData(DataObject *object, bool skipSetup)
{
    if (object == nullptr)
        return false;

    _dataObject = object;

    _httpClient.begin(String(UDP_URL));
    _httpClient.addHeader("Fiware-Service", String(UDP_FIWARE_SERVICE), true);
    _httpClient.addHeader("Fiware-ServicePath", String(UDP_FIWARE_SERVICEPATH));
    _httpClient.addHeader("Content-Type", String(UDP_POST_CONTEXT_TYPE));

    String content = this->parseData();
    int response = _httpClient.POST(content);
    _httpClient.end();

    switch (response)
    {
    case HTTP_CODE_OK:
    case HTTP_CODE_NO_CONTENT:
        PUBLISH_LOG("[HTTP] Success %d\n", response);
        return true;
        break;
    case HTTP_CODE_CREATED:
        PUBLISH_WARN("[HTTP] Entity created on upload.. %d\n", response);
        break;
    case HTTP_CODE_BAD_REQUEST:
    case HTTP_CODE_NOT_FOUND:
        PUBLISH_WARN("[HTTP] HTTP Post error %d\n", response);
        break;
    default:
        PUBLISH_WARN("Unhandled HTTP response: %d\n", response);
        break;
    }

    return false;
}

String UDPTransmission::parseData()
{
    DynamicJsonDocument doc(JSON_POST_DOC_SIZE);

#ifndef GREEN_ROOF
    // Add Battery
    JsonObject entity = doc.createNestedObject(String(BATTERY_LVL_NAME));
    entity["type"] = "Integer";
    entity["value"] = this->_dataObject->batteryLevel;
#endif
    
    // Add sensor data
    for (auto entry : this->_dataObject->items)
    {
        // Add measurement
        JsonObject entity = doc.createNestedObject(entry->fieldName.c_str());
        entity["type"] = "Integer";
        entity["value"] = entry->fieldValue;
    }

    // Json to string
    serializeJson(doc, _parsedData);

    // DEBUG
    PUBLISH_LOG("%s\n", _parsedData.c_str());

    return String(_parsedData);
}
