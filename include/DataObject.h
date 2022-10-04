/**************************************************************
 * @file DataObject.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **************************************************************/
#pragma once
#include <Arduino.h>
#include <vector>

/**************************************************************
 * @brief Data field
 *
 * Key value pair of type Int
 **************************************************************/
struct DataEntry
{
    /**************************************************************
     * @brief Construct a new Data Entry object
     *
     * @param name Field name
     * @param value Field data
     **************************************************************/
    DataEntry(String name, int value) : fieldName(name), fieldValue(value){};

    String fieldName; /*< Field name string */
    int16_t fieldValue;    /*< Field data int */
};

/**************************************************************
 * @brief Data object
 *
 * Object containing a list of DataEntries
 **************************************************************/
struct DataObject
{
    std::vector<DataEntry *> items; /*< List of data fields */
};
