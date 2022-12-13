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
| **Measurement** | **Database Field Name** | **Data**       | **Type**                  | **Size in Bytes** |
| --------------- | ----------------------- | -------------- | ------------------------- | ----------------- |
| Temperature 1   | temp\_in                | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Temperature 2   | temp\_surf              | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Temperature 3   | temp\_out               | Celsius \* 100 | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 1      | moist\_1                | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 2      | moist\_2                | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Moisture 3      | moist\_3                | Promille       | Int\_16 (-32768 to 32767) | 2 bytes           |
| Water Level     | water\_runoff           | mm             | Int\_16 (-32768 to 32767) | 2 bytes           |
| Timestamp       | intTimestamp            | Unix timestamp | Uint\_64                  | 4 bytes           |
| Padding         |                         | \-             | \-                        | 4 bytes           |
|                 |                         |                | **Total:**                | 22 bytes          |

## Class list
| **Class Name**        | **Object Type**         |
| --------------------- | ----------------------- |
| MeasurementController | Module / Sensor Manager |
| DataPublisher         | Data Transmission       |
| TransmissionBase      | Data Transmission       |
| HTTPTransmission      | Data Transmission       |
| IOTHubTransmission    | Data Transmission       |
| UDPTransmission       | Data Transmission       |
| DataObject            | Data Storage            |
| DataEntry             | Data Storage            |
| DataStore             | Data Storage            |
| SensorBase            | Module / Sensor Base    |
| OneWireTemperatureBus | Communication           |
| **Sensors**           |                         |
| TestSensor            | Module  / Sensor        |
| DS18B20Sensor         | Module  / Sensor        |
| MoistureSensor        | Module  / Sensor        |
| WaterLevelSensor      | Module  / Sensor        |

### Azure Json Format
```Json
{
    "intTimestamp": [Unix Timestamp],
    "deviceId": [Device Identifier / Green or Normal],
    "temp_in": [Temp in degrees * 100],
    "temp_surf": [Temp in degrees * 100],
    "temp_out": [Temp in degrees * 100],
    "moist_1": [Moisture in promille],
    "moist_2": [Moisture in promille],
    "moist_3": [Moisture in promille],
    "water_runoff": [Distance in mm from sensor to water level],
    "intBatteryPercentage": [Battery level in percent],
}
```
### Urban Data Platform Format
Structure:
[See Creation Format Green](@ref UDPPage_Create_GR)
Structure:
[See Creation Format Normal](@ref UDPPage_Create_NO)