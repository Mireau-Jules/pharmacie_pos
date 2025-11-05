#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include "database/DatabaseManager.h"
#include "controllers/AuthController.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Utiliser le singleton
    DatabaseManager& dbManager = DatabaseManager::instance();
    
    if (!dbManager.connectToDatabase("pharmacy.db")) {
        QMessageBox::critical(nullptr, "Erreur", 
            "Impossible de se connecter à la base de données!");
        return -1;
    }
    
    if (!dbManager.initializeSchema("database/schema.sql")) {
        QMessageBox::critical(nullptr, "Erreur", 
            "Impossible d'initialiser le schéma de la base de données!");
        return -1;
    }

    // Créer les utilisateurs par défaut avec mots de passe hashés
    AuthController authController;
    
    // Vérifier si les users existent déjà
    QSqlQuery checkQuery("SELECT COUNT(*) FROM users");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        qDebug() << "Creating default users...";
        authController.createUser("admin", "admin123", "Gérant");
        authController.createUser("caissier", "caisse123", "Caissier");
    }

    QMainWindow window;
    window.setWindowTitle("POS Pharmacy - Les 4 Saisons");
    window.resize(1024, 768);
    window.show();

    return app.exec();
}