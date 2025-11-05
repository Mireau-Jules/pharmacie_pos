#include <QApplication>
#include <QMainWindow>
#include "database/DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DatabaseManager dbManager;
    if (!dbManager.connectToDatabase("pharmacy.db")) return -1;
    if (!dbManager.initializeSchema("database/schema.sql")) return -1;

    QMainWindow window;
    window.setWindowTitle("POSPharmacy");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
