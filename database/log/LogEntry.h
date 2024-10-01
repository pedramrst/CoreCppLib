#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <string>
#include "nlohmann/json.hpp"

class LogEntry
{
private:
    static const size_t MAX_MESSAGE_LEN_;

    std::string message_;
    int processingTime_;

public:
    LogEntry(const std::string &message, int processingTime);

    nlohmann::json toJson() const;
    static LogEntry fromJson(const nlohmann::json &j);
    std::string getMessage() const;
    int getPrecessingTime() const;
};

#endif