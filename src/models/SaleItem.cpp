#include "SaleItem.h"

SaleItem::SaleItem() : id(0), saleId(0), productId(0), quantity(0), unitPrice(0.0), subtotal(0.0) {}

SaleItem::SaleItem(int id, int saleId, int productId, int quantity, double unitPrice, double subtotal)
    : id(id), saleId(saleId), productId(productId), quantity(quantity), unitPrice(unitPrice), subtotal(subtotal) {}
