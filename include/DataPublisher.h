/**************************************************************
 * @file DataPublisher.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **************************************************************/
#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>

#include "Settings/config.h"
#include "DataObject.h"

/**************************************************************
 * @brief Wireless data parser and publisher
 *
 **************************************************************/
class DataPublisher
{
public:
    /**************************************************************
     * @brief Singleton class instance
     *
     * @return DataPublisher& Class single instance
     **************************************************************/
    static DataPublisher *INSTANCE();

public:
    /**************************************************************
     * @brief Deleted copy constructor
     *
     **************************************************************/
    DataPublisher(DataPublisher const &) = delete;

    /**************************************************************
     * @brief Deleted = operator
     *
     **************************************************************/
    void operator=(DataPublisher const &) = delete;

    /**************************************************************
     * @brief Connect to the wireless network
     *
     **************************************************************/
    void wirelessConnect();

    /**************************************************************
     * @brief Get connection status
     *
     * @return true Connected
     * @return false Disconnected
     **************************************************************/
    bool wirelessGetConnected();

    /**************************************************************
     * @brief Disconnect from the wireless network
     *
     **************************************************************/
    void wirelessDisconnect();

    /**************************************************************
     * @brief Wireless event callback method
     *
     * @param event Event type
     * @param info Event info
     **************************************************************/
    static void wirelessEvent(WiFiEvent_t event, WiFiEventInfo_t info);

    /**************************************************************
     * @brief Set the Data for transmission
     *
     * @param data Sensor measurement data
     **************************************************************/
    void setData(DataObject data);

    /**************************************************************
     * @brief Add datafield to data object
     *
     * @param fieldName Name of datafield
     * @param fieldValue Value
     **************************************************************/
    void addData(std::string fieldName, int fieldValue);

    /**************************************************************
     * @brief Send data to network
     *
     * @return true Data has been send successfully
     * @return false Data send has failed.
     **************************************************************/
    bool sendData();

    /**************************************************************
     * @brief Format raw data to single sendable string
     *
     * @return std::string Parsed string
     **************************************************************/
    std::string parseData();

private:
    /**************************************************************
     * @brief Private constructor
     *
     **************************************************************/
    DataPublisher();

    /**************************************************************
     * @brief Destroy the Data Publisher object
     *
     **************************************************************/
    virtual ~DataPublisher();

    bool _connected;         /*< Connections status */
    bool _disconnected;      /*< Disconnected during process */
    DataObject *_rawData;    /*< Raw data in struct */
    std::string _parsedData; /*< Parsed data in string */
    HTTPClient _httpClient;  /*< Http Client instance for POST */
};
