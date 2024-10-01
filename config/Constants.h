#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <string>
#include <unordered_map>

namespace Constants
{
    // Postgres - If you would like to change it, you should apply your changes to all configmaps
    extern const char *DEFAULT_DB_CONNECTION_STR;

    // Kafka configs - If you would like to change it, you should apply your changes to all configmaps
    extern const char *DEFAULT_KAFKA_BROKER;

    // Kafka topics and group ids - If you would like to change it, you should check all configmaps and apply necessary changes to some of them
    extern const char *DEFAULT_RAW_TOPIC;
    extern const char *DEFAULT_PREPROCESSOR_TOPIC;
    extern const char *DEFAULT_PREPROCESSOR_GROUP_ID;
    
    // Redis configs - If you would like to change it, you should apply your changes to all configmaps
    extern const char *DEFAULT_REDIS_HOST;
    extern const int DEFAULT_REDIS_PORT;
}

#endif
