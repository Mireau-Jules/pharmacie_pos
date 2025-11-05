#ifndef PRODUCTCONTROLLER_H
#define PRODUCTCONTROLLER_H

#include <QObject>
#include <QVector>
#include "models/Product.h"

class ProductController : public QObject {
    Q_OBJECT

public:
    explicit ProductController(QObject *parent = nullptr);

    QVector<Product> getAllProducts();
    bool addProduct(const Product &product);
    bool updateProduct(const Product &product);
    bool deleteProduct(int productId);
    QVector<Product> searchProduct(const QString &query);
    bool updateStock(int productId, int newStock);
};

#endif // PRODUCTCONTROLLER_H
