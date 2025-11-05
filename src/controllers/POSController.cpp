#include "POSController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

POSController::POSController(QObject *parent) : QObject(parent) {}

bool POSController::processSale(const Sale &sale, const QVector<SaleItem> &items) {
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery saleQuery;
    saleQuery.prepare("INSERT INTO sales (user_id, total, cash_received, change_amount) VALUES (?, ?, ?, ?)");
    saleQuery.addBindValue(sale.userId);
    saleQuery.addBindValue(sale.total);
    saleQuery.addBindValue(sale.cashReceived);
    saleQuery.addBindValue(sale.changeAmount);

    if (!saleQuery.exec()) {
        qCritical() << "Failed to insert sale:" << saleQuery.lastError().text();
        db.rollback();
        return false;
    }

    int saleId = saleQuery.lastInsertId().toInt();

    for (const SaleItem &item : items) {
        QSqlQuery itemQuery;
        itemQuery.prepare("INSERT INTO sale_items (sale_id, product_id, quantity, unit_price, subtotal) VALUES (?, ?, ?, ?, ?)");
        itemQuery.addBindValue(saleId);
        itemQuery.addBindValue(item.productId);
        itemQuery.addBindValue(item.quantity);
        itemQuery.addBindValue(item.unitPrice);
        itemQuery.addBindValue(item.subtotal);

        if (!itemQuery.exec()) {
            qCritical() << "Failed to insert sale item:" << itemQuery.lastError().text();
            db.rollback();
            return false;
        }

        // Update product stock
        QSqlQuery stockQuery;
        stockQuery.prepare("UPDATE products SET stock = stock - ? WHERE id = ?");
        stockQuery.addBindValue(item.quantity);
        stockQuery.addBindValue(item.productId);
        if (!stockQuery.exec()) {
            qCritical() << "Failed to update stock:" << stockQuery.lastError().text();
            db.rollback();
            return false;
        }
    }

    db.commit();
    qDebug() << "Sale processed successfully.";
    return true;
}
