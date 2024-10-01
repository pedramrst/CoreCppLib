#include "RedisHandler.h"

RedisHandler::RedisHandler(const std::string &host, int port)
    : host_(host), port_(port), context_(nullptr) {}

RedisHandler::~RedisHandler()
{
    if (context_ != nullptr)
    {
        redisFree(context_);
    }
}

void RedisHandler::connect()
{
    context_ = redisConnect(host_.c_str(), port_);
    if (context_ == nullptr || context_->err)
    {
        if (context_)
        {
            std::string errstr = context_->errstr;
            redisFree(context_);
            throw std::runtime_error("(RedisHandler) Connection error: " + errstr);
        }
        else
        {
            throw std::runtime_error("(RedisHandler) Connection error: can't allocate redis context");
        }
    }
}

void RedisHandler::set(const std::string &key, const std::string &value)
{
    redisReply *reply = (redisReply *)redisCommand(context_, "SET %s %s", key.c_str(), value.c_str());
    if (reply == nullptr)
    {
        throw std::runtime_error("(RedisHandler) SET command failed");
    }
    freeReplyObject(reply);
}

std::string RedisHandler::get(const std::string &key)
{
    redisReply *reply = (redisReply *)redisCommand(context_, "GET %s", key.c_str());
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL)
    {
        throw std::runtime_error("(RedisHandler) GET command failed or key does not exist");
    }
    std::string value(reply->str, reply->len);
    freeReplyObject(reply);
    return value;
}

bool RedisHandler::exists(const std::string &key)
{
    redisReply *reply = (redisReply *)redisCommand(context_, "EXISTS %s", key.c_str());
    if (reply == nullptr)
    {
        throw std::runtime_error("(RedisHandler) EXISTS command failed");
    }
    bool exists = reply->integer == 1;
    freeReplyObject(reply);
    return exists;
}

void RedisHandler::remove(const std::string &key)
{
    redisReply *reply = (redisReply *)redisCommand(context_, "DEL %s", key.c_str());
    if (reply == nullptr)
    {
        throw std::runtime_error("(RedisHandler) DEL command failed");
    }
    freeReplyObject(reply);
}