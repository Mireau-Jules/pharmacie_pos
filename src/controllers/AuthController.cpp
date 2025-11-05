#include "AuthController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QCryptographicHash>

AuthController::AuthController(QObject *parent) : QObject(parent) {}

QString AuthController::hashPassword(const QString &password) {
    // Utiliser SHA256 pour le hashing (en production, utiliser bcrypt ou Argon2)
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );
    return hash.toHex();
}

bool AuthController::verifyPassword(const QString &password, const QString &hash) {
    return hashPassword(password) == hash;
}

User AuthController::authenticate(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
        qCritical() << "❌ Auth query failed:" << query.lastError().text();
        return User(); // Invalid user
    }

    if (query.next()) {
        QString storedHash = query.value("password_hash").toString();
        
        // Vérifier le mot de passe
        if (verifyPassword(password, storedHash)) {
            qDebug() << "✅ User authenticated:" << username;
            return User(
                query.value("id").toInt(),
                query.value("username").toString(),
                storedHash,
                query.value("role").toString()
            );
        } else {
            qWarning() << "⚠️  Invalid password for user:" << username;
        }
    } else {
        qWarning() << "⚠️  User not found:" << username;
    }

    return User(); // Invalid user (id = 0)
}

bool AuthController::createUser(const QString &username, const QString &password, const QString &role) {
    // Valider le rôle
    if (role != "Gérant" && role != "Caissier") {
        qCritical() << "❌ Invalid role:" << role;
        return false;
    }

    QString passwordHash = hashPassword(password);
    
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash, role) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(passwordHash);
    query.addBindValue(role);

    if (!query.exec()) {
        qCritical() << "❌ Failed to create user:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ User created:" << username;
    return true;
}