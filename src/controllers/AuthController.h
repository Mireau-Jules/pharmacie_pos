#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "models/User.h"

class AuthController : public QObject {
    Q_OBJECT

public:
    explicit AuthController(QObject *parent = nullptr);
    
    User authenticate(const QString &username, const QString &password);
    bool createUser(const QString &username, const QString &password, const QString &role);
    
    // Méthodes utilitaires
    static QString hashPassword(const QString &password);
    static bool verifyPassword(const QString &password, const QString &hash);

private:
    User* currentUser = nullptr;
};

#endif // AUTHCONTROLLER_H