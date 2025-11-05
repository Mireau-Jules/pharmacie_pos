#ifndef POSCONTROLLER_H
#define POSCONTROLLER_H

#include <QObject>
#include <QVector>
#include "models/Sale.h"
#include "models/SaleItem.h"

class POSController : public QObject {
    Q_OBJECT

public:
    explicit POSController(QObject *parent = nullptr);

    bool processSale(const Sale &sale, const QVector<SaleItem> &items);
};

#endif // POSCONTROLLER_H
