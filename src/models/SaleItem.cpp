#include "SaleItem.h"

SaleItem::SaleItem() 
    : id(0), saleId(0), productId(0), productName(""), 
      quantity(0), unitPrice(0.0), subtotal(0.0) {}

SaleItem::SaleItem(int id, int saleId, int productId, const QString &productName, 
                   int quantity, double unitPrice, double subtotal)
    : id(id), saleId(saleId), productId(productId), productName(productName),
      quantity(quantity), unitPrice(unitPrice), subtotal(subtotal) {}