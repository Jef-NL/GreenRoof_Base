/**
 * @file DataStore.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Local data storage object
 * @version 0.1
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>
#include <vector>
#include "FS.h"
#include "SPIFFS.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED false
#include "Settings/config.h"
#include "DataObject.h"
#include "Transmit/TransmissionBase.h"

/**
 * @brief Local data storage class
 *
 **/
class DataStore
{
public:
    /**
     * @brief Construct a new Data Store object
     *
     **/
    DataStore();

    /**
     * @brief Destroy the Data Store object
     *
     **/
    virtual ~DataStore();

public:
#pragma pack(push, 1)
#ifdef GREEN_ROOF
    /**
     * @brief Pre defined storage object for this device.
     *
     **/
    struct StorageBlock
    {
        int16_t temp1;      // 2 Bytes
        int16_t temp2;      // 2 Bytes
        int16_t temp3;      // 2 Bytes
        int16_t moist1;     // 2 Bytes
        int16_t moist2;     // 2 Bytes
        int16_t moist3;     // 2 Bytes
        int16_t waterLvl;   // 2 Bytes
        uint64_t timestamp; // 4 Bytes
    };
#else
    struct StorageBlock
    {
        int16_t temp1;      // 2 Bytes
        int16_t temp2;      // 2 Bytes
        int16_t temp3;      // 2 Bytes
        int16_t waterLvl;   // 2 Bytes
        uint64_t timestamp; // 4 Bytes
    };
#endif
#pragma pack(pop)

    /**
     * @brief Check if data has been stored locally
     *
     * @return true Data available
     * @return false No data available
     **/
    bool dataAvailable();

    /**
     * @brief Add data object to local storage
     *
     * @param data Sensor data object
     **/
    void storeDataObject(DataObject data);

    /**
     * @brief Send retained data from local storage
     *
     * @param endpoint Transmission method instance
     **/
    void transmitDataStorage(TransmissionBase *endpoint);

private:
    /**
     * @brief Format data from raw data into storage data
     *
     * @param data Raw sensor data with names
     * @return StorageBlock Minified data values
     **/
    StorageBlock formatData(DataObject data);

    /**
     * @brief Format data from storage data back to raw data
     *
     * @param returnData Properly fromatted data for transmission
     * @param data Storage data loaded from memory
     **/
    void reFormatData(DataObject &returnData, StorageBlock data);

    /**
     * @brief Add data to local storage file
     *
     * @param data Data object to store
     **/
    void appendData(StorageBlock data);

    /**
     * @brief Load data stored in file into the local _retainedData vector
     *
     **/
    void loadDataStorage();

    /**
     * @brief Delete locally stored data
     *
     * Used after the data has been send to the endpoint.
     **/
    void deleteDataStorage();

    std::vector<StorageBlock> _retainedData; /**< Memory location for data stored within the filesystem */
    fs::FS &_fileSystem;                     /**< File system reference */
};
