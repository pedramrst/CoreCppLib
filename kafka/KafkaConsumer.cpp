#include "KafkaConsumer.h"

KafkaConsumer::KafkaConsumer(const std::string &brokers, const std::string &groupId, const std::string &topic)
    : brokers_(brokers), groupId_(groupId), topic_(topic), consumer_(nullptr), conf_(nullptr), topicConf_(nullptr)
{
    init();
}

KafkaConsumer::~KafkaConsumer()
{
    if (consumer_)
    {
        consumer_->close();
        delete consumer_;
    }
    if (conf_)
    {
        delete conf_;
    }
    if (topicConf_)
    {
        delete topicConf_;
    }
    RdKafka::wait_destroyed(5000);
}

std::string KafkaConsumer::consumeMessage()
{
    if (!consumer_)
    {
        throw std::runtime_error("Consumer is not initialized properly");
    }
    std::string message;
    RdKafka::Message *msg = consumer_->consume(20000);
    if (msg->err() == RdKafka::ERR_NO_ERROR)
    {
        message.assign(static_cast<const char *>(msg->payload()), msg->len());
    }
    else if (msg->err() == RdKafka::ERR__TIMED_OUT)
    {
        throw std::runtime_error("Consumer error: " + msg->errstr());
    }
    delete msg;
    return message;
}

void KafkaConsumer::init()
{
    std::string errstr;

    conf_ = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    topicConf_ = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    if (conf_->set("bootstrap.servers", brokers_, errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set bootstrap.servers: " + errstr);
    }

    if (conf_->set("group.id", groupId_, errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set group.id: " + errstr);
    }

    if (conf_->set("auto.offset.reset", "earliest", errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set auto.offset.reset: " + errstr);
    }

    if (conf_->set("enable.auto.commit", "false", errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set enable.auto.commit: " + errstr);
    }

    consumer_ = RdKafka::KafkaConsumer::create(conf_, errstr);
    if (!consumer_)
    {
        throw std::runtime_error("Failed to create consumer: " + errstr);
    }

    std::vector<std::string> topics = {topic_};
    RdKafka::ErrorCode err = consumer_->subscribe(topics);
    if (err)
    {
        throw std::runtime_error("Failed to subscribe to topic: " + RdKafka::err2str(err));
    }
}
