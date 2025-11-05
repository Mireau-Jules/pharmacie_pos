#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectToDatabase(const QString &dbPath);
    bool initializeSchema(const QString &schemaPath);
    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
