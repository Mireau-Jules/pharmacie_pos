QT += core gui widgets sql printsupport

CONFIG += c++17
CONFIG -= console  # Enlever console pour une app GUI

TEMPLATE = app
TARGET = POSPharmacy

# Définir les répertoires
INCLUDEPATH += src database

# Models
SOURCES += \
    src/models/Product.cpp \
    src/models/User.cpp \
    src/models/Sale.cpp \
    src/models/SaleItem.cpp

HEADERS += \
    src/models/Product.h \
    src/models/User.h \
    src/models/Sale.h \
    src/models/SaleItem.h

# Database
SOURCES += \
    database/DatabaseManager.cpp

HEADERS += \
    database/DatabaseManager.h

# Controllers
SOURCES += \
    src/controllers/AuthController.cpp \
    src/controllers/ProductController.cpp \
    src/controllers/POSController.cpp \
    src/controllers/ReportController.cpp

HEADERS += \
    src/controllers/AuthController.h \
    src/controllers/ProductController.h \
    src/controllers/POSController.h \
    src/controllers/ReportController.h

# Views (TODO: Créer les fichiers .cpp correspondants)
# SOURCES += \
#     src/views/LoginWindow.cpp \
#     src/views/MainWindow.cpp \
#     src/views/ProductWindow.cpp \
#     src/views/AddProductDialog.cpp \
#     src/views/POSWindow.cpp \
#     src/views/PaymentDialog.cpp \
#     src/views/ReportWindow.cpp

# HEADERS += \
#     src/views/LoginWindow.h \
#     src/views/MainWindow.h \
#     src/views/ProductWindow.h \
#     src/views/AddProductDialog.h \
#     src/views/POSWindow.h \
#     src/views/PaymentDialog.h \
#     src/views/ReportWindow.h

# Utils
SOURCES += \
    src/utils/ReceiptPrinter.cpp

HEADERS += \
    src/utils/ReceiptPrinter.h

# Main
SOURCES += \
    src/main.cpp

# UI Forms
FORMS += \
    src/views/LoginWindow.ui \
    src/views/MainWindow.ui \
    src/views/ProductWindow.ui \
    src/views/AddProductDialog.ui \
    src/views/POSWindow.ui \
    src/views/PaymentDialog.ui \
    src/views/ReportWindow.ui

# Resources (si vous avez un .qrc)
# RESOURCES += resources/resources.qrc

# Copier la base de données au build
copydata.commands = $(COPY_DIR) $$PWD/database $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata