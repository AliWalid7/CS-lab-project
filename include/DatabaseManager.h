#pragma once

#include <sqlite3.h>
#include <string>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase();

    void saveMessage(
        const std::string& sender,
        const std::string& receiver,
        const std::string& message
    );

private:
    sqlite3* db;
};