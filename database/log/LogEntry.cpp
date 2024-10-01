#include "LogEntry.h"

const size_t LogEntry::MAX_MESSAGE_LEN_ = 128;

LogEntry::LogEntry(const std::string &message, int processingTime)
    : processingTime_(processingTime)
{
    message_ = message.length() > MAX_MESSAGE_LEN_ ? message.substr(0, MAX_MESSAGE_LEN_) : message;
}

nlohmann::json LogEntry::toJson() const
{
    return nlohmann::json{
        {"message", message_},
        {"processingTime", processingTime_}};
}

LogEntry LogEntry::fromJson(const nlohmann::json &j)
{
    return LogEntry{
        j.at("message").get<std::string>(),
        j.at("processingTime").get<int>()};
}

std::string LogEntry::getMessage() const
{
    return message_;
}

int LogEntry::getPrecessingTime() const
{
    return processingTime_;
}