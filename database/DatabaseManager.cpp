#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection closed";
    }
}

bool DatabaseManager::connectToDatabase(const QString &dbPath) {
    if (connected) {
        qWarning() << "Database already connected";
        return true;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "❌ Failed to open database:" << db.lastError().text();
        return false;
    }

    connected = true;
    qDebug() << "✅ Database connected at" << dbPath;
    return true;
}

bool DatabaseManager::initializeSchema(const QString &schemaPath) {
    if (!connected) {
        qCritical() << "❌ Database not connected";
        return false;
    }

    QFile file(schemaPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "❌ Failed to open schema file:" << schemaPath;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

    // Utiliser une transaction pour tout ou rien
    if (!db.transaction()) {
        qCritical() << "❌ Failed to start transaction";
        return false;
    }

    QSqlQuery query(db);
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    
    for (const QString &statement : statements) {
        QString trimmed = statement.trimmed();
        if (!trimmed.isEmpty() && !trimmed.startsWith("--")) {
            if (!query.exec(trimmed)) {
                qCritical() << "❌ SQL error:" << query.lastError().text();
                qCritical() << "Failed statement:" << trimmed;
                db.rollback();
                return false;
            }
        }
    }

    if (!db.commit()) {
        qCritical() << "❌ Failed to commit schema";
        db.rollback();
        return false;
    }

    qDebug() << "✅ Database schema initialized successfully";
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() const {
    return db;
}

bool DatabaseManager::isConnected() const {
    return connected && db.isOpen();
}