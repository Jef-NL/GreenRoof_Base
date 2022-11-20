#pragma once
#include <Arduino.h>

// Enable debugging
#define PROTO_DEBUG_LVL 3

// Log messages
#if PROTO_DEBUG_LVL == 1
#define DEBUG_LOG(...)
#define NETWORK_LOG(...) Serial.printf( "[NETWORK] > " __VA_ARGS__)
#define PUBLISH_LOG(...) Serial.printf( "[PUB] > " __VA_ARGS__)
#define MEASURE_LOG(...)
#elif PROTO_DEBUG_LVL == 2
#define DEBUG_LOG(...) Serial.printf(__VA_ARGS__)
#define NETWORK_LOG(...) Serial.printf( "[NETWORK] > " __VA_ARGS__)
#define PUBLISH_LOG(...) Serial.printf( "[PUB] > " __VA_ARGS__)
#define MEASURE_LOG(...)
#elif PROTO_DEBUG_LVL == 3
#define DEBUG_LOG(...) Serial.printf(__VA_ARGS__)
#define NETWORK_LOG(...) Serial.printf( "[NETWORK] > " __VA_ARGS__)
#define PUBLISH_LOG(...) Serial.printf( "[PUB] > " __VA_ARGS__)
#define MEASURE_LOG(...) Serial.printf( "[M] > " __VA_ARGS__)
#else
#define DEBUG_LOG(...)
#define NETWORK_LOG(...)
#define PUBLISH_LOG(...)
#define MEASURE_LOG(...)
#endif

// Warnings
#if PROTO_DEBUG_LVL != -1
#define NETWORK_WARN(...) Serial.printf( "# ! # > NETWORK WARN >>> " __VA_ARGS__)
#define PUBLISH_WARN(...) Serial.printf( "# ! # > PUBLISH WARN >>> " __VA_ARGS__)
#define DEBUG_WARN(...) Serial.printf( "# ! # > WARN >>> " __VA_ARGS__)
#else
#define NETWORK_WARN(...)
#define PUBLISH_WARN(...)
#define DEBUG_WARN(...)
#endif