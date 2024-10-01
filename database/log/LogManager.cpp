#include "LogManager.h"
#include <iostream>

LogManager::LogManager(PostgresHandler &dbHandler) : dbHandler_(dbHandler)
{
    preparedInsertStatement_ = PQprepare(dbHandler_.getConnection(), "insert_log",
                                         "INSERT INTO logs (module_name, log_time, log_level, message, processing_time) VALUES ($1, $2, $3, $4, $5)", 0, nullptr);
    if (!preparedInsertStatement_ || PQresultStatus(preparedInsertStatement_) != PGRES_COMMAND_OK)
    {
        throw std::runtime_error("(LogManager) Preparation of INSERT statement failed");
    }
}

LogManager::~LogManager()
{
    PQclear(preparedInsertStatement_);
}

void LogManager::insertLog(const std::string &moduleName, const std::string &time,
                           const std::string &logLevel, const std::string &message,
                           int processingTime)
{
    const char *paramValues[5];
    int paramLengths[5];
    int paramFormats[5];
    paramValues[0] = moduleName.c_str();
    paramValues[1] = time.c_str();
    paramValues[2] = logLevel.c_str();
    paramValues[3] = message.c_str();
    paramValues[4] = std::to_string(processingTime).c_str();
    for (int i = 0; i < 5; i++)
    {
        paramLengths[i] = 0; // Not needed for text format
        paramFormats[i] = 0; // Text format
    }

    PGresult *res = PQexecPrepared(dbHandler_.getConnection(), "insert_log", 5, paramValues, paramLengths, paramFormats, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        throw std::runtime_error("(LogManager) INSERT command failed: " + std::string(PQerrorMessage(dbHandler_.getConnection())));
    }
    PQclear(res);
}