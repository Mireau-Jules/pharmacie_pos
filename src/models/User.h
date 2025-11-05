#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    int id;
    QString username;
    QString passwordHash;
    QString role;

    User();
    User(int id, const QString &username, const QString &passwordHash, const QString &role);
};

#endif // USER_H
