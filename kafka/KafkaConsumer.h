#ifndef KAFKACONSUMER_H
#define KAFKACONSUMER_H

#include <string>
#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include <stdexcept>

class KafkaConsumer
{
public:
    KafkaConsumer(const std::string &brokers, const std::string &groupId, const std::string &topic);
    ~KafkaConsumer();
    std::string consumeMessage();

private:
    void init();

    std::string brokers_;
    std::string groupId_;
    std::string topic_;
    RdKafka::KafkaConsumer *consumer_;
    RdKafka::Conf *conf_;
    RdKafka::Conf *topicConf_;
};

#endif // KAFKACONSUMER_H