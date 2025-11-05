#include "User.h"

User::User() : id(0), username(""), passwordHash(""), role("") {}

User::User(int id, const QString &username, const QString &passwordHash, const QString &role)
    : id(id), username(username), passwordHash(passwordHash), role(role) {}
