#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
public:
    // Singleton pattern
    static DatabaseManager& instance();
    
    bool connectToDatabase(const QString &dbPath);
    bool initializeSchema(const QString &schemaPath);
    QSqlDatabase getDatabase() const;
    bool isConnected() const;
    
    // Empêcher la copie
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    
    QSqlDatabase db;
    bool connected = false;
};

#endif // DATABASEMANAGER_H