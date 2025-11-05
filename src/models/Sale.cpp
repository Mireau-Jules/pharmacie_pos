#include "Sale.h"

Sale::Sale() : id(0), userId(0), total(0.0), cashReceived(0.0), changeAmount(0.0) {}

Sale::Sale(int id, int userId, double total, double cashReceived, double changeAmount, const QDateTime &createdAt)
    : id(id), userId(userId), total(total), cashReceived(cashReceived), changeAmount(changeAmount), createdAt(createdAt) {}
