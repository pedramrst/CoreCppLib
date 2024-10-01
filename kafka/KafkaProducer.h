#ifndef KAFKAPRODUCER_H
#define KAFKAPRODUCER_H

#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <string>
#include <stdexcept>

class KafkaProducer
{
public:
    KafkaProducer(const std::string &brokers, const std::string topic);
    ~KafkaProducer();

    void produce(const std::string &message);

private:
    std::string topic_;
    RdKafka::Producer *producer_;

    bool topicExists();
};

#endif