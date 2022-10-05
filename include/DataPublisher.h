/**
 * @file DataPublisher.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Data parser and management for transmission and storage of sensor data
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <ArduinoJson.h>

#include "Settings/config.h"
#include "DataObject.h"

#define NTP_SERVER "ch.pool.ntp.org"
#define TZ_INFO "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00" // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

/**
 * @brief Wireless data parser and publisher
 *
 **/
class DataPublisher
{
public:
    /**
     * @brief Singleton class instance
     *
     * @return DataPublisher& Class single instance
     **/
    static DataPublisher *INSTANCE();

public:
    /**
     * @brief Deleted copy constructor
     *
     **/
    DataPublisher(DataPublisher const &) = delete;

    /**
     * @brief Deleted = operator
     *
     **/
    void operator=(DataPublisher const &) = delete;

    /**
     * @brief Connect to the wireless network
     *
     **/
    void wirelessConnect();

    /**
     * @brief Get connection status
     *
     * @return true Connected
     * @return false Disconnected
     **/
    bool wirelessGetConnected();

    /**
     * @brief Disconnect from the wireless network
     *
     **/
    void wirelessDisconnect();

    /**
     * @brief Wireless event callback method
     *
     * @param event Event type
     * @param info Event info
     **/
    static void wirelessEvent(WiFiEvent_t event, WiFiEventInfo_t info);

    /**
     * @brief Set the Data for transmission
     *
     * @param data Sensor measurement data
     **/
    void setData(DataObject data);

    /**
     * @brief Add datafield to data object
     *
     * @param fieldName Name of datafield
     * @param fieldValue Value
     **/
    void addData(String fieldName, int fieldValue);

    /**
     * @brief Send data to network
     *
     * @return true Data has been send successfully
     * @return false Data send has failed.
     **/
    bool sendData();

private:
    /**
     * @brief Private constructor
     *
     **/
    DataPublisher();

    /**
     * @brief Destroy the Data Publisher object
     *
     **/
    virtual ~DataPublisher();

    /**
     * @brief Send HTTP post to server with sensor data
     *
     * @return true Post was send successfully
     * @return false Post has failed to send
     **/
    bool sendHTTPPost();

    /**
     * @brief Format raw data to single sendable string
     *
     * @return String Parsed string
     **/
    String parseData();

    /**
     * @brief Update the current Timestamp for a measurement
     *
     * @return time_t Timestamp in seconds since 1 Jan 1970 UTC
     **/
    time_t updateTimestamp();

    /**
     * @brief Helper print function
     *
     * @param localTime Timestamp to print
     **/
    void showTime(tm localTime);

    bool _connected;        /**< Connections status */
    bool _disconnected;     /**< Disconnected during process */
    DataObject *_rawData;   /**< Raw data in struct */
    String _parsedData;     /**< Parsed data in string */
    HTTPClient _httpClient; /**< Http Client instance for POST */
    time_t _lastTimestamp;  /**< Timestamp of last measurement */
};
