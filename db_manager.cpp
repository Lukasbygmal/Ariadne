#include "db_manager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string &server, const std::string &username,
                                 const std::string &password, const std::string &database)
    : server(server), username(username), password(password), database(database), driver(nullptr)
{
    try
    {
        driver = get_driver_instance();
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error initializing MySQL driver: " << e.what() << std::endl;
    }
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

bool DatabaseManager::connect()
{
    try
    {
        if (conn && !conn->isClosed())
        {
            return true; // Already connected
        }

        // Create a connection
        conn.reset(driver->connect(server, username, password));

        // Set the schema
        conn->setSchema(database);

        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException in connect(): " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        std::cerr << "ErrorCode: " << e.getErrorCode() << std::endl;
        return false;
    }
}

void DatabaseManager::disconnect()
{
    if (conn && !conn->isClosed())
    {
        conn->close();
    }
}

bool DatabaseManager::loadPlayer(Player &player, int user_id)
{
    if (!connect())
    {
        return false;
        std::cout << "Failed to connect!" << std::endl;
    }

    try
    {
        // Prepare statement for selecting data
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM Users WHERE user_id = ?"));

        pstmt->setString(1, std::to_string(user_id)); // pstmt->setInt(1, userId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next())
        {
            // Create a new player instance with retrieved data
            Player loadedPlayer(res->getString("name"), res->getInt("lvl"), res->getInt("xp"), res->getInt("gold"), res->getInt("max_hp"), res->getInt("strength_stat"), res->getInt("agility_stat"), res->getInt("armor_stat"));
            int currentLvl = loadedPlayer.getLevel();

            loadedPlayer.healToMax();

            player = loadedPlayer;

            return true;
        }
        else
        {
            std::cerr << "Player with id: " << user_id << "not found" << std::endl;
            return false;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException in loadPlayer(): " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        std::cerr << "ErrorCode: " << e.getErrorCode() << std::endl;
        return false;
    }
}

bool DatabaseManager::doesPlayerExist(const std::string &playerName)
{ // need to rethink as it should use user_id as key, so can have repeat names?
    if (!connect())
    {
        return false;
    }

    try
    {
        // Prepare statement for checking if player exists
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT COUNT(*) FROM Users WHERE name = ?"));

        pstmt->setString(1, playerName);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next())
        {
            return res->getInt(1) > 0;
        }

        return false;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException in doesPlayerExist(): " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        std::cerr << "ErrorCode: " << e.getErrorCode() << std::endl;
        return false;
    }
}