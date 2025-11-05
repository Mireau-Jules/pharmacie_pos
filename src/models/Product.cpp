#include "Product.h"

Product::Product() : id(0), price(0.0), stock(0) {}

Product::Product(int id, const QString &name, const QString &barcode, double price, int stock)
    : id(id), name(name), barcode(barcode), price(price), stock(stock) {}
