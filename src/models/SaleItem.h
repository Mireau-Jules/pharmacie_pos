#ifndef SALEITEM_H
#define SALEITEM_H

#include <QString>

class SaleItem {
public:
    int id;
    int saleId;
    int productId;
    QString productName;
    int quantity;
    double unitPrice;
    double subtotal;

    SaleItem();
    SaleItem(int id, int saleId, int productId, const QString &productName, 
             int quantity, double unitPrice, double subtotal);
    
    // Calculer le subtotal automatiquement
    void calculateSubtotal() { subtotal = quantity * unitPrice; }
};

#endif // SALEITEM_H