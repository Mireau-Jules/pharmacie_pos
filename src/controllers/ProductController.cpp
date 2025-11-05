#include "ProductController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>

ProductController::ProductController(QObject *parent) : QObject(parent) {}

QVector<Product> ProductController::getAllProducts() {
    QVector<Product> products;
    QSqlQuery query("SELECT * FROM products");

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

bool ProductController::addProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("INSERT INTO products (name, barcode, price, stock) VALUES (?, ?, ?, ?)");
    query.addBindValue(product.name);
    query.addBindValue(product.barcode);
    query.addBindValue(product.price);
    query.addBindValue(product.stock);
    return query.exec();
}

bool ProductController::updateProduct(const Product &product) {
    QSqlQuery query;
    query.prepare("UPDATE products SET name = ?, barcode = ?, price = ?, stock = ? WHERE id = ?");
    query.addBindValue(product.name);
    query.addBindValue(product.barcode);
    query.addBindValue(product.price);
    query.addBindValue(product.stock);
    query.addBindValue(product.id);
    return query.exec();
}

bool ProductController::deleteProduct(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id = ?");
    query.addBindValue(productId);
    return query.exec();
}

QVector<Product> ProductController::searchProduct(const QString &queryText) {
    QVector<Product> results;
    QSqlQuery query;
    query.prepare("SELECT * FROM products WHERE name LIKE ? OR barcode LIKE ?");
    QString pattern = "%" + queryText + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    query.exec();

    while (query.next()) {
        Product p(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("barcode").toString(),
            query.value("price").toDouble(),
            query.value("stock").toInt()
        );
        results.append(p);
    }

    return results;
}

bool ProductController::updateStock(int productId, int newStock) {
    QSqlQuery query;
    query.prepare("UPDATE products SET stock = ? WHERE id = ?");
    query.addBindValue(newStock);
    query.addBindValue(productId);
    return query.exec();
}