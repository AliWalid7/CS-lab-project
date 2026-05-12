#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::openDatabase() {
    int rc = sqlite3_open("chat.db", &db);

    if (rc != SQLITE_OK) {
        std::cout << "Cannot open database: "
                  << (db ? sqlite3_errmsg(db) : "unknown error")
                  << "\n";

        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }

        return false;
    }

    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sender TEXT NOT NULL,"
        "receiver TEXT NOT NULL,"
        "message TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cout << "SQL Error creating messages table: "
                  << (errMsg ? errMsg : "unknown SQL error")
                  << "\n";

        if (errMsg) {
            sqlite3_free(errMsg);
        }

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
    if (!db) {
        std::cout << "Database is not open. Cannot save message.\n";
        return;
    }

    const char* sql =
        "INSERT INTO messages(sender, receiver, message) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cout << "SQL prepare error: "
                  << (db ? sqlite3_errmsg(db) : "unknown error")
                  << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, sender.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, receiver.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, message.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cout << "SQL insert error: "
                  << (db ? sqlite3_errmsg(db) : "unknown error")
                  << "\n";
    } else {
        std::cout << "Message saved\n";
    }

    sqlite3_finalize(stmt);
}
