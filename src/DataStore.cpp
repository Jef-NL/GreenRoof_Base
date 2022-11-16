/**
 * @file DataStore.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Local data storage object
 * @version 0.1
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "DataStore.h"

DataStore::DataStore() : _fileSystem(SPIFFS)
{
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("SPIFFS Mount Failed");
    }
}

DataStore::~DataStore()
{
}

bool DataStore::dataAvailable()
{
    return _fileSystem.exists(STORAGE_FILE);
}

void DataStore::storeDataObject(DataObject data)
{
    Serial.println("Storing measurement locally.");
    StorageBlock storeData = this->formatData(data);
    this->appendData(storeData);
#ifdef GREEN_ROOF
    Serial.printf("Wrote: %d, %d, %d, %d, %d, %d, %d, %llu\n",
                  storeData.temp1,
                  storeData.temp2,
                  storeData.temp3,
                  storeData.waterLvl,
                  storeData.moist1,
                  storeData.moist2,
                  storeData.moist3,
                  storeData.timestamp);
#else
    Serial.printf("Wrote: %d, %d, %d, %d, %llu\n",
                  storeData.temp1,
                  storeData.temp2,
                  storeData.temp3,
                  storeData.waterLvl,
                  storeData.timestamp);
#endif
}

void DataStore::transmitDataStorage(TransmissionBase *endpoint)
{
    // Load data in memory
    this->loadDataStorage(); // ToDo: Split up into chunks to limit the time and prevent memory overflows

    for (auto object : _retainedData)
    {
        DataObject transmissionData = {0};
        this->reFormatData(transmissionData, object);
        if (!endpoint->transmitData(&transmissionData, true))
        {
            Serial.println("Failed to send data from storage");
            return;
        }
    }

    // Delete after sucessfull send
    this->deleteDataStorage();
}

DataStore::StorageBlock DataStore::formatData(DataObject data)
{
    StorageBlock returnData = {0};
    returnData.timestamp = data.timestamp;

    for (auto entry : data.items)
    {
        if (strcmp(entry->fieldName.c_str(), SENS_TEMP1_NAME) == 0)
            returnData.temp1 = entry->fieldValue;
        else if (strcmp(entry->fieldName.c_str(), SENS_TEMP2_NAME) == 0)
            returnData.temp2 = entry->fieldValue;
        else if (strcmp(entry->fieldName.c_str(), SENS_TEMP3_NAME) == 0)
            returnData.temp3 = entry->fieldValue;
        else if (strcmp(entry->fieldName.c_str(), SENS_WATER_NAME) == 0)
            returnData.waterLvl = entry->fieldValue;
#ifdef GREEN_ROOF
        else if (strcmp(entry->fieldName.c_str(), SENS_MOIST1_NAME) == 0)
            returnData.moist1 = entry->fieldValue;
        else if (strcmp(entry->fieldName.c_str(), SENS_MOIST2_NAME) == 0)
            returnData.moist2 = entry->fieldValue;
        else if (strcmp(entry->fieldName.c_str(), SENS_MOIST3_NAME) == 0)
            returnData.moist3 = entry->fieldValue;
#endif
        else
        {
            Serial.println("INCORRECT DATA SAVE");
        }
    }

    return returnData;
}

void DataStore::reFormatData(DataObject &returnData, DataStore::StorageBlock data)
{
    returnData.timestamp = data.timestamp;
    returnData.items.push_back(new DataEntry(SENS_TEMP1_NAME, data.temp1));
    returnData.items.push_back(new DataEntry(SENS_TEMP2_NAME, data.temp2));
    returnData.items.push_back(new DataEntry(SENS_TEMP3_NAME, data.temp3));
    returnData.items.push_back(new DataEntry(SENS_WATER_NAME, data.waterLvl));
#ifdef GREEN_ROOF
    returnData.items.push_back(new DataEntry(SENS_MOIST1_NAME, data.moist1));
    returnData.items.push_back(new DataEntry(SENS_MOIST2_NAME, data.moist2));
    returnData.items.push_back(new DataEntry(SENS_MOIST3_NAME, data.moist3));
#endif
}

void DataStore::appendData(DataStore::StorageBlock data)
{
    File file = _fileSystem.open(STORAGE_FILE, FILE_APPEND);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }

    if (file.write((uint8_t *)&data, sizeof(DataStore::StorageBlock)))
    {
        Serial.println("- Data stored locally");
    }
    else
    {
        Serial.println("- Data store failed...");
    }
    file.close();
}

void DataStore::loadDataStorage()
{
    File file = _fileSystem.open(STORAGE_FILE);
    size_t fileSize = 0;
    uint16_t items = 0;
    if (!file)
    {
        Serial.println("- failed to open file for Reading");
        return;
    }
    else
    {
        fileSize = file.size();
        Serial.printf("File size: %d\n", fileSize);
    }

    // Calculate Items
    items = floor(fileSize / sizeof(StorageBlock));
    StorageBlock data = {0};

    for (size_t i = 0; i < items; i++)
    {
        file.seek(sizeof(StorageBlock) * i);

        if (file.read((uint8_t *)&data, sizeof(StorageBlock)))
        {
#ifdef GREEN_ROOF
            Serial.printf("Read(%d): %d, %d, %d, %d, %d, %d, %d, %llu\n",
                          i,
                          data.temp1,
                          data.temp2,
                          data.temp3,
                          data.waterLvl,
                          data.moist1,
                          data.moist2,
                          data.moist3,
                          data.timestamp);
#else
            Serial.printf("Read(%d): %d, %d, %d, %d, %llu\n",
                          i,
                          data.temp1,
                          data.temp2,
                          data.temp3,
                          data.waterLvl,
                          data.timestamp);
#endif

            _retainedData.push_back(data);
        }
        else
        {
            Serial.println("Read failed");
        }
    }

    file.close();
}

void DataStore::deleteDataStorage()
{
    if (_fileSystem.remove(STORAGE_FILE))
    {
        Serial.println("- file deleted");
    }
    else
    {
        Serial.println("- delete failed");
    }
}