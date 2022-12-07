/**
 * @file UDPTransmission.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Transmission class for publishing to Urban Data Platform
 * @version 0.1
 * @date 2022-12-06
 * 
 * @copyright Copyright (c) 2022
 * 
**/
#pragma once
#include "TransmissionBase.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

/**
 * @brief UDP / HTTP post transmission
 * 
**/
class UDPTransmission : public TransmissionBase
{
public:
    /**
     * @brief Construct a new UDPTransmission object
     * 
    **/
    UDPTransmission();

    /**
     * @brief Destroy the UDPTransmission object
     * 
    **/
    virtual ~UDPTransmission();

public:
    /**
     * @brief Transmit data using HTTP Post method to UDP
     *
     * @param object Measurement struct
     * @param skipSetup Skip steps for instant transmit
     * @return true Transmission successfull
     * @return false Transmission failed.
     */
    bool transmitData(DataObject *object, bool skipSetup = false) override;

private:
    /**
     * @brief Prase raw data into Json
     *
     * @return String Parsed data as string.
     */
    String parseData() override;

    /**
     * @brief Parse time to required ISO8601 format
     * 
     * @param unixTimestamp Timestamp in Unix format
     * @return String Timestamp string
    **/
    String parseTime(time_t unixTimestamp);

    HTTPClient _httpClient; /**< HTTP client instance */
};

