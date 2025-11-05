#include "AuthController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

AuthController::AuthController(QObject *parent) : QObject(parent) {}

User AuthController::authenticate(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ? AND password_hash = ?");
    query.addBindValue(username);
    query.addBindValue(password); // In production, hash this!

    if (query.exec() && query.next()) {
        return User(
            query.value("id").toInt(),
            query.value("username").toString(),
            query.value("password_hash").toString(),
            query.value("role").toString()
        );
    }

    return User(); // id = 0 means invalid
}
