#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "../PostgresHandler.h"
#include <string>

class LogManager
{
private:
    PostgresHandler &dbHandler_;
    PGresult *preparedInsertStatement_;

public:
    // LogManager();
    LogManager(PostgresHandler &dbHandler);
    ~LogManager();

    void insertLog(const std::string &moduleName, const std::string &time,
                   const std::string &logLevel, const std::string &message,
                   int processingTime);
};

#endif