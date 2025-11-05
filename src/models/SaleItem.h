#ifndef SALEITEM_H
#define SALEITEM_H

class SaleItem {
public:
    int id;
    int saleId;
    int productId;
    int quantity;
    double unitPrice;
    double subtotal;

    SaleItem();
    SaleItem(int id, int saleId, int productId, int quantity, double unitPrice, double subtotal);
};

#endif // SALEITEM_H
