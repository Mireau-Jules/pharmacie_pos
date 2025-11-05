#ifndef REPORTCONTROLLER_H
#define REPORTCONTROLLER_H

#include <QObject>
#include <QVector>
#include "models/Product.h"
#include "models/Sale.h"

class ReportController : public QObject {
    Q_OBJECT

public:
    explicit ReportController(QObject *parent = nullptr);

    QVector<Sale> getTodaySales();
    QVector<Product> getLowStockProducts(int threshold = 5);
};

#endif // REPORTCONTROLLER_H
