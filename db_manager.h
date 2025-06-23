#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>
#include <memory>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Player.h"

class DatabaseManager
{
private:
    std::string server;
    std::string username;
    std::string password;
    std::string database;

    sql::Driver *driver;
    std::unique_ptr<sql::Connection> conn;

    bool connect();
    void disconnect();

    bool doesPlayerExist(const std::string &playerName);

public:
    DatabaseManager(const std::string &server, const std::string &username,
                    const std::string &password, const std::string &database);
    ~DatabaseManager();

    bool loadPlayer(Player &player, int userId);
    bool savePlayer(Player &player, int userId);
};

#endif