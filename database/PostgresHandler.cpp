#include "PostgresHandler.h"

PostgresHandler::PostgresHandler() {}

PostgresHandler::PostgresHandler(const std::string &connectionString)
    : connectionString_(connectionString), connection_(nullptr) {}

PostgresHandler::~PostgresHandler()
{
    disconnect();
}

void PostgresHandler::connect()
{
    connection_ = PQconnectdb(connectionString_.c_str());
    checkConnection();
}

void PostgresHandler::disconnect()
{
    if (connection_)
    {
        PQfinish(connection_);
        connection_ = nullptr;
    }
}

PGresult *PostgresHandler::executeQuery(const std::string &query)
{
    if (!connection_)
    {
        throw std::runtime_error("(PostgresHandler) No active connection to the database.");
    }
    PGresult *result = PQexec(connection_, query.c_str());
    throwIfError(result);
    return result;
}

void PostgresHandler::clearResult(PGresult *result)
{
    PQclear(result);
}

bool PostgresHandler::isConnected() const
{
    return connection_ != nullptr && PQstatus(connection_) == CONNECTION_OK;
}

PGconn *PostgresHandler::getConnection() const
{
    return connection_;
}

void PostgresHandler::checkConnection()
{
    if (PQstatus(connection_) != CONNECTION_OK)
    {
        std::string errorMsg = PQerrorMessage(connection_);
        disconnect();
        throw std::runtime_error("(PostgresHandler) Connection to database failed: " + errorMsg);
    }
}

void PostgresHandler::throwIfError(PGresult *result)
{
    ExecStatusType status = PQresultStatus(result);
    if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK)
    {
        std::string errorMsg = PQresultErrorMessage(result);
        PQclear(result);
        throw std::runtime_error("(PostgresHandler) Query failed: " + errorMsg);
    }
}