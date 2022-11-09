/**
 * @file IOTHubTransmission.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief IOT Hub based transmission method
 * @version 0.1
 * @date 2022-11-04
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include "TransmissionBase.h"
#include "Settings/config.h"

#include <mqtt_client.h>

// C99 libraries
#include <cstdlib>
#include <string.h>
#include <time.h>
#include <ArduinoJson.h>
// Azure IoT SDK for C includes
#include <az_core.h>
#include <az_iot.h>

#include "AzIoTSasToken.h"

class IOTHubTransmission : public TransmissionBase
{
public:
    /**
     * @brief Construct a new IOTHubTransmission object
     *
     **/
    IOTHubTransmission();

    /**
     * @brief Destroy the IOTHubTransmission object
     *
     **/
    virtual ~IOTHubTransmission();

public:
    /**
     * @brief Transmit data using HTTP Post method
     *
     * @param object Measurement struct
     * @param skipSetup Skip steps for instant transmit
     * @return true Transmission successfull
     * @return false Transmission failed.
     */
    bool transmitData(DataObject *object, bool skipSetup = false) override;

    /**
     * @brief Close connection with endpoint
     * 
    **/
    void close() override;

    /**
     * @brief MQTT event handler method
     * 
     * @param event MQTT Event
     * @return esp_err_t Error on fault
    **/
    static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);

private:
    /**
     * @brief Prase raw data into Json
     *
     * @return String Parsed data as string.
     */
    String parseData() override;

    /**
     * @brief Initialise IOT Hub client instance
     *
     **/
    void initIotHubClient();

    /**
     * @brief Initialise MQTT client instance
     *
     * @return int Success code 0 = OK
     **/
    int initMqttClient();
};
