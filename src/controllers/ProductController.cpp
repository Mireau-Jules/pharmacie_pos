#include "ProductController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>

ProductController::ProductController(QObject *parent) : QObject(parent) {}

bool ProductController::validateProduct(const Product &product, QString &errorMsg) {
    if (product.name.trimmed().isEmpty()) {
        errorMsg = "Le nom du produit ne peut pas être vide";
        return false;
    }
    
    if (product.price <= 0) {
        errorMsg = "Le prix doit être supérieur à 0";
        return false;
    }
    
    if (product.stock < 0) {
        errorMsg = "Le stock ne peut pas être négatif";
        return false;
    }
    
    return true;
}

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
    QString errorMsg;
    if (!validateProduct(product, errorMsg)) {
        qCritical() << "❌ Validation failed:" << errorMsg;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO products (name, barcode, price, stock) VALUES (?, ?, ?, ?)");
    query.addBindValue(product.name.trimmed());
    query.addBindValue(product.barcode.trimmed());
    query.addBindValue(product.price);
    query.addBindValue(product.stock);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to add product:" << query.lastError().text();
        
        if (query.lastError().text().contains("UNIQUE")) {
            qCritical() << "Code-barres déjà existant";
        }
        return false;
    }

    qDebug() << "✅ Product added with ID:" << query.lastInsertId().toInt();
    return true;
}

bool ProductController::updateProduct(const Product &product) {
    QString errorMsg;
    if (!validateProduct(product, errorMsg)) {
        qCritical() << "❌ Validation failed:" << errorMsg;
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE products SET name = ?, barcode = ?, price = ?, stock = ? WHERE id = ?");
    query.addBindValue(product.name.trimmed());
    query.addBindValue(product.barcode.trimmed());
    query.addBindValue(product.price);
    query.addBindValue(product.stock);
    query.addBindValue(product.id);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to update product:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ProductController::deleteProduct(int productId) {
    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id = ?");
    query.addBindValue(productId);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to delete product:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Product> ProductController::searchProduct(const QString &queryText) {
    QVector<Product> results;
    
    if (queryText.trimmed().isEmpty()) {
        return getAllProducts();
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM products WHERE name LIKE ? OR barcode LIKE ?");
    QString pattern = "%" + queryText + "%";
    query.addBindValue(pattern);
    query.addBindValue(pattern);
    
    if (!query.exec()) {
        qCritical() << "❌ Search failed:" << query.lastError().text();
        return results;
    }

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

    qDebug() << "✅ Search returned" << results.size() << "products";
    return results;
}

bool ProductController::updateStock(int productId, int newStock) {
    if (newStock < 0) {
        qCritical() << "❌ Stock cannot be negative";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE products SET stock = ? WHERE id = ?");
    query.addBindValue(newStock);
    query.addBindValue(productId);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to update stock:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qWarning() << "⚠️ Product not found:" << productId;
        return false;
    }

    qDebug() << "✅ Stock updated for product" << productId;
    return true;
}