/**
 * @file MeasurementController.cpp
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Main measurement and communication controller
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#include "MeasurementController.h"

MeasurementController::MeasurementController()
{
    _publisher = DataPublisher::INSTANCE();
}

MeasurementController::~MeasurementController()
{
}

void MeasurementController::addSensor(SensorBase::Sensor *sensor)
{
    _sensors.push_back(sensor);
}

void MeasurementController::addSensors(std::vector<SensorBase::Sensor *> sensors)
{
    for (auto sensor : sensors)
    {
        this->addSensor(sensor);
    }
}

bool MeasurementController::runProcess() {
    // ToDo: Make threaded (Maybe, is interrupt based for networking) and add return status
    this->connect();
    this->runMeasurements();
    this->publishMeasurements();
    
    return true;
}

void MeasurementController::connect()
{
    _publisher->wirelessConnect();
}

bool MeasurementController::runMeasurements()
{
    if (_sensors.size() <= 0)
        return false;

    for (auto sensorReg : _sensors)
    {
        // Run measurement for all sensors
        if (!sensorReg->sensor->startMeasurement())
            return false;
    }

    return true;
}

bool MeasurementController::publishMeasurements()
{
    if (_sensors.size() <= 0)
        return false;

    for (auto sensorReg : _sensors)
    {
        // Add sensor data to publisher
        _publisher->addData(sensorReg->sensorName, sensorReg->sensor->getMeasurementResult());
    }

    return _publisher->sendData();
}
