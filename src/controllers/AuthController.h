#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "models/User.h"

class AuthController : public QObject {
    Q_OBJECT

public:
    explicit AuthController(QObject *parent = nullptr);
    User authenticate(const QString &username, const QString &password);
};

#endif // AUTHCONTROLLER_H
