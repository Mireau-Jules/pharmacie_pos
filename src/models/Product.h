#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product {
public:
    int id;
    QString name;
    QString barcode;
    double price;
    int stock;

    Product(); // constructeur vide
    Product(int id, const QString &name, const QString &barcode, double price, int stock);
};

#endif // PRODUCT_H
