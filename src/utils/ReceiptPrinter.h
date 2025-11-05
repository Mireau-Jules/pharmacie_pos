#ifndef RECEIPTPRINTER_H
#define RECEIPTPRINTER_H

#include <QString>
#include <QVector>
#include "models/Sale.h"
#include "models/SaleItem.h"

class ReceiptPrinter {
public:
    static QString generateReceipt(const Sale &sale, const QVector<SaleItem> &items);
};

#endif // RECEIPTPRINTER_H
