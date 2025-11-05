#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    int id;
    QString username;
    QString passwordHash;
    QString role;

    User(); // Invalid user
    User(int id, const QString &username, const QString &passwordHash, const QString &role);
    
    bool isValid() const { return id > 0; }
    bool isManager() const { return role == "Gérant"; }
    bool isCashier() const { return role == "Caissier"; }
};

#endif // USER_H