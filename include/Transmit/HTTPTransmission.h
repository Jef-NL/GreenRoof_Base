/**
 * @file HTTPTransmission.h
 * @author Jef (hj.baars@student.avans.nl)
 * @brief Transmission method for HTTP post transfers
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "TransmissionBase.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

/**
 * @brief HTTP Post transmission
 * 
 */
class HTTPTransmission : public TransmissionBase
{
public:
    /**
     * @brief Construct a new HTTPTransmission object
     * 
     */
    HTTPTransmission();

    /**
     * @brief Destroy the HTTPTransmission object
     * 
     */
    virtual ~HTTPTransmission();

public:
    /**
     * @brief Transmit data using HTTP Post method
     * 
     * @param object Measurement struct
     * @return true Transmission successfull
     * @return false Transmission failed.
     */
    bool transmitData(DataObject *object) override;

private:
    /**
     * @brief Prase raw data into Json
     * 
     * @return String Parsed data as string.
     */
    String parseData() override;

    HTTPClient _httpClient; /**< HTTP client instance */
};
