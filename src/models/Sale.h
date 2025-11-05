#ifndef SALE_H
#define SALE_H

#include <QString>
#include <QDateTime>

class Sale {
public:
    int id;
    int userId;
    double total;
    double cashReceived;
    double changeAmount;
    QDateTime createdAt;

    Sale();
    Sale(int id, int userId, double total, double cashReceived, double changeAmount, const QDateTime &createdAt);
    
    bool isValid() const { return id > 0; }
};

#endif // SALE_H