#ifndef REDISHANDLER_H
#define REDISHANDLER_H

#include <iostream>
#include <string>
#include <hiredis/hiredis.h>
#include <stdexcept>
#include <vector>

class RedisHandler
{
public:
    RedisHandler(const std::string &host, int port);
    ~RedisHandler();

    void connect();
    void set(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
    bool exists(const std::string &key);
    void remove(const std::string &key);

private:
    std::string host_;
    int port_;
    redisContext *context_;
};

#endif