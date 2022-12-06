# Green Roof Prototype
Embedded solution for sensory and monitoring of green and non green roof setup.

## Sensor creation
``` cpp
MeasurementController *controller;
controller = new MeasurementController();
controller->addSensor(new SensorBase::Sensor([Name], new [Derived class]));
```

## Data structure
Local data storage format.
| **Measurement** | **Data**       | **Type**                  | **Size in Bytes** |
| --------------- | -------------- | ------------------------- | ----------------- |
| Temperature 1   | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Temperature 2   | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Temperature 3   | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 1      | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 2      | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 3      | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Water Level     | mm             | Int\_16 (-32768 to 32767) | 2 bytes           |
| Timestamp       | Unix timestamp | Uint\_64                  | 4 bytes           |
| Padding         | \-             | \-                        | 4 bytes           |
|                 |                | **Total:**                | 22 bytes          |

## Class list
| **Class Name**        | **Object Type**         |
| --------------------- | ----------------------- |
| MeasurementController | Module / Sensor Manager |
| DataPublisher         | Data Transmission       |
| DataObject            | Data Storage            |
| DataEntry             | Data Storage            |
| DataStore             | Data Storage            |
| SensorBase            | Module / Sensor Base    |
| OneWireTemperatureBus | Communication           |
| **Sensors**           |
| TestSensor            | Module  / Sensor        |
| DS18B20Sensor         | Module  / Sensor        |
| MoistureSensor        | Module  / Sensor        |
| WaterLevelSensor      | Module  / Sensor        |