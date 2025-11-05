#include "ReceiptPrinter.h"

QString ReceiptPrinter::generateReceipt(const Sale &sale, const QVector<SaleItem> &items) {
    QString receipt;
    receipt += "=== PHARMACY RECEIPT ===\n";
    receipt += "Date: " + sale.createdAt.toString("yyyy-MM-dd hh:mm:ss") + "\n";
    receipt += "Cashier ID: " + QString::number(sale.userId) + "\n\n";

    receipt += "Items:\n";
    for (const SaleItem &item : items) {
        receipt += QString("%1 x%2 @ %.2f = %.2f\n")
            .arg(item.productId)
            .arg(item.quantity)
            .arg(item.unitPrice)
            .arg(item.subtotal);
    }

    receipt += "\nTotal: " + QString::number(sale.total, 'f', 2) + "\n";
    receipt += "Cash: " + QString::number(sale.cashReceived, 'f', 2) + "\n";
    receipt += "Change: " + QString::number(sale.changeAmount, 'f', 2) + "\n";
    receipt += "========================\n";

    return receipt;
}
