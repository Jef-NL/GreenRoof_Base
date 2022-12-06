# Urban Data Platform entities
Json objects for the entity creation of the green and normal roof.

## General HTTP
### Service Path
UDP project service path used for the Green and Normal roof.
> /green_roof_proto

#### Headers
> "Content-Type": "application/json"
> "Fiware-Service": "demoiot"
> "Fiware-ServicePath": "/green_roof_proto"

#### HTTP Response
| **HTTP Response**     | **Result**         |
| --------------------- | ------------------ |
| 201                   | Entity Created     |
| 204                   | Entity Updated     |
| 400                   | Bad Request        |
| 404                   | Entity Unknown     |

### Green Roof
#### Create
```http
POST 20.16.84.167:1026/v2/entities
```
```json
{
    "id": "green_roof",
    "type": "MultiSensor",
    "water_runoff": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_in": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_surf": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_out": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "moist_1": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "moist_2": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "moist_3": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    }
}
```
#### Get values
```http
GET 20.16.84.167:1026/v2/entities/green_roof
```

### Normal Roof
#### Create
```http
POST 20.16.84.167:1026/v2/entities
```
```json
{
    "id": "normal_roof",
    "type": "MultiSensor",
    "water_runoff": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_in": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_surf": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "temp_out": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    },
    "intBatteryPercentage": {
        "metadata": {},
        "type": "Integer",
        "value": 0
    }
}
```
#### Get values
```http
GET 20.16.84.167:1026/v2/entities/normal_roof
```