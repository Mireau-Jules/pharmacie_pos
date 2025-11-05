#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::connectToDatabase(const QString &dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected at" << dbPath;
    return true;
}

bool DatabaseManager::initializeSchema(const QString &schemaPath) {
    QFile file(schemaPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open schema file:" << schemaPath;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

    QSqlQuery query;
    for (const QString &statement : sql.split(';', Qt::SkipEmptyParts)) {
        QString trimmed = statement.trimmed();
        if (!trimmed.isEmpty()) {
            if (!query.exec(trimmed)) {
                qCritical() << "SQL error:" << query.lastError().text();
                qCritical() << "Failed statement:" << trimmed;
                return false;
            }
        }
    }

    qDebug() << "Database schema initialized.";
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() const {
    return db;
}
