#include "ReportController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>

ReportController::ReportController(QObject *parent) : QObject(parent) {}

QVector<Sale> ReportController::getTodaySales() {
    QVector<Sale> sales;
    QSqlQuery query;
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    query.prepare("SELECT * FROM sales WHERE DATE(created_at) = ?");
    query.addBindValue(today);
    query.exec();

    while (query.next()) {
        Sale s(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("total").toDouble(),
            query.value("cash_received").toDouble(),
            query.value("change_amount").toDouble(),
            query.value("created_at").toDateTime()
        );
        sales.append(s);
    }

    return sales;
}

QVector<Product> ReportController::getLowStockProducts(int threshold) {
    QVector<Product> products;
    QSqlQuery query;
    query.prepare("SELECT * FROM products WHERE stock <= ?");
    query.addBindValue(threshold);
    query.exec();

    while (query.next()) {
        Product p(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("barcode").toString(),
            query.value("price").toDouble(),
            query.value("stock").toInt()
        );
        products.append(p);
    }

    return products;
}
