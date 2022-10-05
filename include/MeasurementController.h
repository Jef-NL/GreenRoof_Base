/**
 * @file MeasurementController.h
 * @author Jef B (hj.baars@student.avans.nl)
 * @brief Main measurement and communication controller
 * @version 0.1
 * @date 2022-10-03
 *
 * @copyright Copyright (c) 2022
 *
 **/
#pragma once
#include <Arduino.h>
#include <vector>

#include "DataObject.h"
#include "DataPublisher.h"
#include "Sensors/SensorBase.h"

/**
 * @brief Controller to execute the sensor measurements and
 * publish the data to the DataPublisher
 *
 **/
class MeasurementController
{
public:
    /**
     * @brief Default constructor
     *
     **/
    MeasurementController();

    /**
     * @brief Destroy the Measurement Controller object
     *
     **/
    virtual ~MeasurementController();

public:
    /**
     * @brief Add one sensor to the controller
     *
     * @param sensor SensorBase::Sensor entry
     **/
    void addSensor(SensorBase::Sensor *sensor);

    /**
     * @brief Add multiple sensors to the controller
     *
     * @param sensors SensorBase::Sensor entrys vector
     **/
    void addSensors(std::vector<SensorBase::Sensor *> sensors);

    /**
     * @brief Run threaded process for measurements
     *
     * This method connects with the network, executes the measurements and publishes them.
     * @return true Run sucessfully
     * @return false Run failed
     **/
    bool runProcess();

private:
    /**
     * @brief Setup Data publisher connection
     *
     **/
    void connect();

    /**
     * @brief Run measurements for all sensors.
     *
     * @return true Measurements completed and connected
     * @return false Measurements not completed or not connected
     **/
    bool runMeasurements();

    /**
     * @brief Publish measurements to the DataPublisher
     *
     * @return true Data sucessfully published
     * @return false Data failed to publish
     **/
    bool publishMeasurements();

    DataPublisher *_publisher;                  /*< Data published instance */
    std::vector<SensorBase::Sensor *> _sensors; /*< Sensors list */
};
