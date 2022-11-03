/**
 * @file TransmissionBase.h
 * @author Jef (hj.baars@student.avans.nl)
 * @brief Transmission base object
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "DataObject.h"
#include "Settings/config.h"

/**
 * @brief Base class for sending data to one of the endpoints.
 * 
 */
class TransmissionBase
{
public:
    /**
     * Default constructor
    */
    TransmissionBase() = default;

    /**
     * Default destructor
    */
    virtual ~TransmissionBase() = default;

public:
    /**
     * @brief Transmit data using method
     * 
     * @param object Measurement struct
     * @return true Transmission successfull
     * @return false Transmission failed.
     */
    virtual bool transmitData(DataObject *object) = 0;

private:
    /**
     * @brief Prase raw data into correct format
     * 
     * @return String Parsed data as string.
     */
    virtual String parseData() = 0;

protected:
    DataObject* _dataObject;    /**< Raw data storage object */
    String _parsedData;         /**< Parsed data string */
};
