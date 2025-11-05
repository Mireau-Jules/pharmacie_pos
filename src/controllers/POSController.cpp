#include "POSController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

POSController::POSController(QObject *parent) : QObject(parent) {}

bool POSController::addToCart(const Product &product, int quantity) {
    if (quantity <= 0) {
        emit errorOccurred("La quantité doit être supérieure à 0");
        return false;
    }
    
    if (product.stock < quantity) {
        emit errorOccurred("Stock insuffisant pour " + product.name);
        return false;
    }
    
    // Vérifier si le produit existe déjà dans le panier
    for (int i = 0; i < cart.size(); ++i) {
        if (cart[i].product.id == product.id) {
            int newQty = cart[i].quantity + quantity;
            if (newQty > product.stock) {
                emit errorOccurred("Stock insuffisant");
                return false;
            }
            cart[i].updateQuantity(newQty);
            emit cartUpdated();
            return true;
        }
    }
    
    // Ajouter nouveau produit au panier
    cart.append(CartItem(product, quantity));
    emit cartUpdated();
    return true;
}

bool POSController::removeFromCart(int index) {
    if (index < 0 || index >= cart.size()) {
        return false;
    }
    
    cart.remove(index);
    emit cartUpdated();
    return true;
}

bool POSController::updateCartItemQuantity(int index, int newQuantity) {
    if (index < 0 || index >= cart.size()) {
        return false;
    }
    
    if (newQuantity <= 0) {
        return removeFromCart(index);
    }
    
    if (newQuantity > cart[index].product.stock) {
        emit errorOccurred("Stock insuffisant");
        return false;
    }
    
    cart[index].updateQuantity(newQuantity);
    emit cartUpdated();
    return true;
}

void POSController::clearCart() {
    cart.clear();
    emit cartUpdated();
}

double POSController::getTotal() const {
    return calculateTotal();
}

double POSController::calculateTotal() const {
    double total = 0.0;
    for (const CartItem &item : cart) {
        total += item.subtotal;
    }
    return total;
}

bool POSController::validateSale(double cashReceived, QString &errorMsg) {
    if (cart.isEmpty()) {
        errorMsg = "Le panier est vide";
        return false;
    }
    
    double total = calculateTotal();
    if (cashReceived < total) {
        errorMsg = QString("Montant insuffisant. Total: %1 HTG, Reçu: %2 HTG")
                    .arg(total, 0, 'f', 2)
                    .arg(cashReceived, 0, 'f', 2);
        return false;
    }
    
    return true;
}

bool POSController::checkStockAvailability(int productId, int quantity) {
    QSqlQuery query;
    query.prepare("SELECT stock FROM products WHERE id = ?");
    query.addBindValue(productId);
    
    if (query.exec() && query.next()) {
        int availableStock = query.value(0).toInt();
        return availableStock >= quantity;
    }
    
    return false;
}

bool POSController::processSale(int userId, double cashReceived, int &saleId, QString &errorMsg) {
    // Validation
    if (!validateSale(cashReceived, errorMsg)) {
        emit errorOccurred(errorMsg);
        return false;
    }
    
    double total = calculateTotal();
    double change = cashReceived - total;
    
    QSqlDatabase db = QSqlDatabase::database();
    
    // Démarrer une transaction
    if (!db.transaction()) {
        errorMsg = "Impossible de démarrer la transaction";
        emit errorOccurred(errorMsg);
        return false;
    }
    
    try {
        // 1. Créer la vente
        QSqlQuery saleQuery;
        saleQuery.prepare("INSERT INTO sales (user_id, total, cash_received, change_amount) "
                         "VALUES (?, ?, ?, ?)");
        saleQuery.addBindValue(userId);
        saleQuery.addBindValue(total);
        saleQuery.addBindValue(cashReceived);
        saleQuery.addBindValue(change);
        
        if (!saleQuery.exec()) {
            throw std::runtime_error("Échec création vente: " + 
                                    saleQuery.lastError().text().toStdString());
        }
        
        saleId = saleQuery.lastInsertId().toInt();
        
        // 2. Ajouter les items et mettre à jour les stocks
        for (const CartItem &item : cart) {
            // Ajouter l'item
            QSqlQuery itemQuery;
            itemQuery.prepare("INSERT INTO sale_items (sale_id, product_id, quantity, unit_price, subtotal) "
                             "VALUES (?, ?, ?, ?, ?)");
            itemQuery.addBindValue(saleId);
            itemQuery.addBindValue(item.product.id);
            itemQuery.addBindValue(item.quantity);
            itemQuery.addBindValue(item.product.price);
            itemQuery.addBindValue(item.subtotal);
            
            if (!itemQuery.exec()) {
                throw std::runtime_error("Échec ajout item: " + 
                                        itemQuery.lastError().text().toStdString());
            }
            
            // Mettre à jour le stock
            QSqlQuery stockQuery;
            stockQuery.prepare("UPDATE products SET stock = stock - ? WHERE id = ?");
            stockQuery.addBindValue(item.quantity);
            stockQuery.addBindValue(item.product.id);
            
            if (!stockQuery.exec()) {
                throw std::runtime_error("Échec mise à jour stock: " + 
                                        stockQuery.lastError().text().toStdString());
            }
        }
        
        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }
        
        qDebug() << "✅ Vente traitée avec succès. ID:" << saleId;
        emit saleCompleted(saleId, total, change);
        clearCart();
        
        return true;
        
    } catch (const std::exception &e) {
        db.rollback();
        errorMsg = QString::fromStdString(e.what());
        qCritical() << "❌ Erreur transaction:" << errorMsg;
        emit errorOccurred(errorMsg);
        return false;
    }
}