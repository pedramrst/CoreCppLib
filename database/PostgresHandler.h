#ifndef POSTGRESHANDLER_H
#define POSTGRESHANDLER_H

#include <postgresql/libpq-fe.h>
#include <string>
#include <stdexcept>
// #include <exceptions/PostgresException.h>

class PostgresHandler
{
public:
    PGconn *connection_;

    PostgresHandler();
    PostgresHandler(const std::string &connectionString);
    ~PostgresHandler();

    void connect();
    void disconnect();
    PGresult *executeQuery(const std::string &query);
    void clearResult(PGresult *result);
    bool isConnected() const;
    PGconn *getConnection() const;

private:
    std::string connectionString_;

    void checkConnection();
    void throwIfError(PGresult *result);
};

#endif