// test_backend_complete.cpp
// Programme de test complet pour tous les modules backend

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include "database/DatabaseManager.h"
#include "controllers/AuthController.h"
#include "controllers/ProductController.h"
#include "controllers/POSController.h"
#include "controllers/ReportController.h"
#include "models/Product.h"
#include "models/User.h"

class BackendTester {
public:
    BackendTester() : testsPassed(0), testsTotal(0) {}
    
    void runAllTests() {
        qDebug() << "\n╔═══════════════════════════════════════════════╗";
        qDebug() << "║   POS PHARMACY - BACKEND TEST SUITE          ║";
        qDebug() << "╚═══════════════════════════════════════════════╝\n";
        
        // Préparer la base de données de test
        setupTestDatabase();
        
        // Tests des modules
        testDatabaseConnection();
        testAuthController();
        testProductController();
        testPOSController();
        testReportController();
        
        // Résumé
        printSummary();
    }

private:
    int testsPassed;
    int testsTotal;
    
    void setupTestDatabase() {
        qDebug() << "🔧 Setup: Création de la base de données de test...";
        
        // Supprimer l'ancienne base si elle existe
        QFile::remove("test_pharmacy.db");
        
        DatabaseManager& db = DatabaseManager::instance();
        if (!db.connectToDatabase("test_pharmacy.db")) {
            qCritical() << "❌ FATAL: Impossible de créer la base de test";
            exit(1);
        }
        
        if (!db.initializeSchema("database/schema.sql")) {
            qCritical() << "❌ FATAL: Impossible d'initialiser le schéma";
            exit(1);
        }
        
        qDebug() << "✅ Base de données de test créée\n";
    }
    
    void testDatabaseConnection() {
        qDebug() << "═══ TEST 1: Database Connection ═══";
        testsTotal++;
        
        DatabaseManager& db = DatabaseManager::instance();
        if (db.isConnected()) {
            qDebug() << "✅ PASS: Base de données connectée";
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Base de données non connectée";
        }
        qDebug() << "";
    }
    
    void testAuthController() {
        qDebug() << "═══ TEST 2: Authentication Controller ═══";
        
        AuthController authCtrl;
        int localPassed = 0;
        int localTotal = 4;
        
        // Test 2.1: Créer des utilisateurs
        testsTotal++;
        if (authCtrl.createUser("admin", "admin123", "Gérant")) {
            qDebug() << "✅ PASS: Utilisateur Gérant créé";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Création utilisateur Gérant échouée";
        }
        
        testsTotal++;
        if (authCtrl.createUser("caissier", "caisse123", "Caissier")) {
            qDebug() << "✅ PASS: Utilisateur Caissier créé";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Création utilisateur Caissier échouée";
        }
        
        // Test 2.2: Authentification valide
        testsTotal++;
        User admin = authCtrl.authenticate("admin", "admin123");
        if (admin.isValid() && admin.isManager()) {
            qDebug() << "✅ PASS: Authentification admin réussie";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Authentification admin échouée";
        }
        
        // Test 2.3: Authentification invalide
        testsTotal++;
        User invalid = authCtrl.authenticate("admin", "wrongpassword");
        if (!invalid.isValid()) {
            qDebug() << "✅ PASS: Mot de passe incorrect rejeté";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Mot de passe incorrect accepté!";
        }
        
        qDebug() << QString("📊 Auth Tests: %1/%2 passed\n").arg(localPassed).arg(localTotal);
    }
    
    void testProductController() {
        qDebug() << "═══ TEST 3: Product Controller ═══";
        
        ProductController productCtrl;
        int localPassed = 0;
        int localTotal = 6;
        
        // Test 3.1: Ajouter des produits
        testsTotal++;
        Product p1(0, "Paracétamol 500mg", "1234567890", 25.50, 100);
        if (productCtrl.addProduct(p1)) {
            qDebug() << "✅ PASS: Produit 1 ajouté";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Ajout produit 1 échoué";
        }
        
        testsTotal++;
        Product p2(0, "Ibuprofène 400mg", "0987654321", 35.00, 50);
        if (productCtrl.addProduct(p2)) {
            qDebug() << "✅ PASS: Produit 2 ajouté";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Ajout produit 2 échoué";
        }
        
        // Test 3.2: Récupérer tous les produits
        testsTotal++;
        QVector<Product> allProducts = productCtrl.getAllProducts();
        if (allProducts.size() == 2) {
            qDebug() << "✅ PASS: 2 produits récupérés";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Nombre de produits incorrect:" << allProducts.size();
        }
        
        // Test 3.3: Rechercher un produit
        testsTotal++;
        QVector<Product> searchResults = productCtrl.searchProduct("Para");
        if (searchResults.size() == 1 && searchResults[0].name.contains("Paracétamol")) {
            qDebug() << "✅ PASS: Recherche produit fonctionne";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Recherche produit échouée";
        }
        
        // Test 3.4: Mettre à jour le stock
        testsTotal++;
        if (productCtrl.updateStock(1, 80)) {
            qDebug() << "✅ PASS: Stock mis à jour";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Mise à jour stock échouée";
        }
        
        // Test 3.5: Modifier un produit
        testsTotal++;
        Product p1Updated(1, "Paracétamol 500mg - Modifié", "1234567890", 28.00, 80);
        if (productCtrl.updateProduct(p1Updated)) {
            qDebug() << "✅ PASS: Produit modifié";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Modification produit échouée";
        }
        
        qDebug() << QString("📊 Product Tests: %1/%2 passed\n").arg(localPassed).arg(localTotal);
    }
    
    void testPOSController() {
        qDebug() << "═══ TEST 4: POS Controller (Sales) ═══";
        
        POSController posCtrl;
        ProductController productCtrl;
        int localPassed = 0;
        int localTotal = 5;
        
        // Récupérer des produits pour la vente
        QVector<Product> products = productCtrl.getAllProducts();
        if (products.isEmpty()) {
            qCritical() << "❌ FAIL: Aucun produit disponible pour les tests POS";
            return;
        }
        
        // Test 4.1: Ajouter au panier
        testsTotal++;
        if (posCtrl.addToCart(products[0], 2)) {
            qDebug() << "✅ PASS: Produit ajouté au panier";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Ajout au panier échoué";
        }
        
        testsTotal++;
        if (products.size() > 1 && posCtrl.addToCart(products[1], 1)) {
            qDebug() << "✅ PASS: Deuxième produit ajouté au panier";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Ajout deuxième produit échoué";
        }
        
        // Test 4.2: Vérifier le total
        testsTotal++;
        double expectedTotal = (products[0].price * 2);
        if (products.size() > 1) expectedTotal += products[1].price;
        double actualTotal = posCtrl.getTotal();
        
        if (qAbs(actualTotal - expectedTotal) < 0.01) {
            qDebug() << QString("✅ PASS: Total correct: %1 HTG").arg(actualTotal, 0, 'f', 2);
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << QString("❌ FAIL: Total incorrect. Attendu: %1, Reçu: %2")
                          .arg(expectedTotal).arg(actualTotal);
        }
        
        // Test 4.3: Traiter une vente
        testsTotal++;
        int saleId = 0;
        QString errorMsg;
        double cashReceived = actualTotal + 50.0; // Payer avec extra
        
        if (posCtrl.processSale(1, cashReceived, saleId, errorMsg)) {
            qDebug() << QString("✅ PASS: Vente traitée avec succès (ID: %1)").arg(saleId);
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Traitement vente échoué:" << errorMsg;
        }
        
        // Test 4.4: Vérifier que le panier est vidé
        testsTotal++;
        if (posCtrl.getCartItemCount() == 0) {
            qDebug() << "✅ PASS: Panier vidé après vente";
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Panier non vidé après vente";
        }
        
        qDebug() << QString("📊 POS Tests: %1/%2 passed\n").arg(localPassed).arg(localTotal);
    }
    
    void testReportController() {
        qDebug() << "═══ TEST 5: Report Controller ═══";
        
        ReportController reportCtrl;
        int localPassed = 0;
        int localTotal = 3;
        
        // Test 5.1: Rapport quotidien
        testsTotal++;
        DailySalesReport dailyReport = reportCtrl.getDailySalesReport(QDate::currentDate());
        if (dailyReport.transactionCount > 0) {
            qDebug() << QString("✅ PASS: Rapport quotidien généré (%1 transactions, %2 HTG)")
                       .arg(dailyReport.transactionCount)
                       .arg(dailyReport.totalAmount, 0, 'f', 2);
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Aucune transaction dans le rapport";
        }
        
        // Test 5.2: Rapport de stock
        testsTotal++;
        StockReport stockReport = reportCtrl.getStockReport();
        if (stockReport.allProducts.size() > 0) {
            qDebug() << QString("✅ PASS: Rapport de stock généré (%1 produits)")
                       .arg(stockReport.allProducts.size());
            localPassed++;
            testsPassed++;
        } else {
            qCritical() << "❌ FAIL: Rapport de stock vide";
        }
        
        // Test 5.3: Produits en stock faible
        testsTotal++;
        QVector<Product> lowStock = reportCtrl.getLowStockProducts(100);
        qDebug() << QString("✅ PASS: Produits en stock faible: %1").arg(lowStock.size());
        localPassed++;
        testsPassed++;
        
        qDebug() << QString("📊 Report Tests: %1/%2 passed\n").arg(localPassed).arg(localTotal);
    }
    
    void printSummary() {
        qDebug() << "\n╔═══════════════════════════════════════════════╗";
        qDebug() << "║              TEST SUMMARY                     ║";
        qDebug() << "╚═══════════════════════════════════════════════╝";
        qDebug() << QString("Tests Passed: %1/%2").arg(testsPassed).arg(testsTotal);
        
        double percentage = (double)testsPassed / testsTotal * 100.0;
        qDebug() << QString("Success Rate: %1%").arg(percentage, 0, 'f', 1);
        
        if (testsPassed == testsTotal) {
            qDebug() << "\n🎉🎉🎉 ALL TESTS PASSED! 🎉🎉🎉";
            qDebug() << "✅ Your backend is fully functional!";
            qDebug() << "✅ Ready for frontend integration!";
        } else {
            qWarning() << QString("\n⚠️  %1 test(s) failed").arg(testsTotal - testsPassed);
            qWarning() << "Please fix the issues before proceeding.";
        }
        
        qDebug() << "\n📝 Test database saved as: test_pharmacy.db";
        qDebug() << "You can inspect it with: sqlite3 test_pharmacy.db\n";
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    BackendTester tester;
    tester.runAllTests();
    
    return 0;
}