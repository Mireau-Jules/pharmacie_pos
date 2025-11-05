#ifndef POSCONTROLLER_H
#define POSCONTROLLER_H

#include <QObject>
#include <QVector>
#include "models/Product.h"
#include "models/Sale.h"
#include "models/SaleItem.h"

// Structure pour gérer les items du panier
struct CartItem {
    Product product;
    int quantity;
    double subtotal;
    
    CartItem(const Product &p, int qty) : product(p), quantity(qty) {
        subtotal = p.price * qty;
    }
    
    void updateQuantity(int qty) {
        quantity = qty;
        subtotal = product.price * qty;
    }
};

class POSController : public QObject {
    Q_OBJECT

public:
    explicit POSController(QObject *parent = nullptr);
    
    // Gestion du panier
    bool addToCart(const Product &product, int quantity = 1);
    bool removeFromCart(int index);
    bool updateCartItemQuantity(int index, int newQuantity);
    void clearCart();
    
    // Getters
    QVector<CartItem> getCart() const { return cart; }
    double getTotal() const;
    int getCartItemCount() const { return cart.size(); }
    
    // Traitement de la vente
    bool processSale(int userId, double cashReceived, int &saleId, QString &errorMsg);
    
    // Vérification du stock
    bool checkStockAvailability(int productId, int quantity);

signals:
    void cartUpdated();
    void saleCompleted(int saleId, double total, double change);
    void errorOccurred(const QString &error);

private:
    QVector<CartItem> cart;
    double calculateTotal() const;
    bool validateSale(double cashReceived, QString &errorMsg);
};

#endif // POSCONTROLLER_H