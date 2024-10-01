#ifndef POSTGRESSINK_H
#define POSTGRESSINK_H

#include <chrono>
#include <memory>
#include <string>
#include <mutex>
#include <iomanip>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"
#include "nlohmann/json.hpp"
#include "database/log/LogManager.h"
#include "../database/log/LogEntry.h"

template <typename Mutex>
class PostgresSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    explicit PostgresSink(LogManager &logManager, const std::string moduleName)
        : logManager_(logManager), moduleName_(moduleName)
    {
    }

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        std::ostringstream time = getCurrentTime();
        std::string logLevel = spdlog::level::to_string_view(msg.level).data();
        std::string strLog(msg.payload.begin(), msg.payload.end());
        nlohmann::json jsonLog = nlohmann::json::parse(strLog);
        LogEntry log = LogEntry::fromJson(jsonLog);
        logManager_.insertLog(moduleName_, time.str(), logLevel,
                              log.getMessage(), log.getPrecessingTime());
    }
    void flush_() override {}
    void set_pattern_(const std::string &pattern) override {}
    void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter) override {}

private:
    LogManager &logManager_;
    std::string moduleName_;
    std::ostringstream getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::ostringstream time_stream;
        time_stream << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
        time_stream << '.' << std::setw(3) << std::setfill('0') << now_ms.count();
        return time_stream;
    }
};

using postgres_sink_mt = PostgresSink<std::mutex>;

#endif