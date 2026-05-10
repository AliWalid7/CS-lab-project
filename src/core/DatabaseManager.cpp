#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::openDatabase() {
    int rc = sqlite3_open("chat.db", &db);

    if (rc) {
        std::cout << "Cannot open database\n";
        return false;
    }

    std::cout << "Database opened successfully\n";
    return true;
}

void DatabaseManager::saveMessage(
    const std::string& sender,
    const std::string& receiver,
    const std::string& message
) {
    std::string sql =
        "INSERT INTO messages(sender, receiver, message) VALUES('" +
        sender + "','" +
        receiver + "','" +
        message + "');";

    char* errMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cout << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    } else {
        std::cout << "Message saved\n";
    }
}