#include "KafkaProducer.h"

KafkaProducer::KafkaProducer(const std::string &brokers, const std::string topic) : topic_(topic)
{
    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("metadata.broker.list", brokers, errstr);
    producer_ = RdKafka::Producer::create(conf, errstr);
    if (!producer_)
    {
        throw std::runtime_error("(KafkaProducer) Producer creation failed: " + errstr);
    }
    if (!topicExists())
    {
        throw std::runtime_error("(KafkaProducer) Producing messages failed: topic did not create");
    }
    delete conf;
}

KafkaProducer::~KafkaProducer()
{
    producer_->flush(10 * 1000);
    delete producer_;
}

void KafkaProducer::produce(const std::string &message)
{
    RdKafka::ErrorCode err = producer_->produce(topic_,
                                                RdKafka::Topic::PARTITION_UA,
                                                RdKafka::Producer::RK_MSG_COPY,
                                                const_cast<char *>(message.c_str()), message.size(),
                                                NULL, 0,
                                                0,
                                                NULL,
                                                NULL);
    if (err != RdKafka::ERR_NO_ERROR)
    {
        throw std::runtime_error("(KafkaProducer) Failed to produce message: " + RdKafka::err2str(err));
    }
}

bool KafkaProducer::topicExists()
{
    RdKafka::Metadata *metadata;
    RdKafka::ErrorCode err = producer_->metadata(true, nullptr, &metadata, 5000);
    if (err != RdKafka::ERR_NO_ERROR)
    {
        throw std::runtime_error("(KafkaProducer) Failed to fetch metadata: " + RdKafka::err2str(err));
    }

    const RdKafka::Metadata::TopicMetadataVector *topics = metadata->topics();
    for (const auto &t : *topics)
    {
        if (t->topic() == topic_)
        {
            delete metadata;
            return true;
        }
    }

    delete metadata;
    return false;
}