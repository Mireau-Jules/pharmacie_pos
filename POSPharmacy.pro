QT += core gui widgets sql

CONFIG += c++17 console

TEMPLATE = app
TARGET = POSPharmacy

SOURCES += \
    src/main.cpp \
    src/models/Product.cpp \
    src/models/User.cpp \
    src/models/Sale.cpp \
    src/models/SaleItem.cpp \
    src/controllers/AuthController.cpp \
    src/controllers/ProductController.cpp \
    src/controllers/POSController.cpp \
    src/controllers/ReportController.cpp

HEADERS += \
    src/models/Product.h \
    src/models/User.h \
    src/models/Sale.h \
    src/models/SaleItem.h \
    src/controllers/AuthController.h \
    src/controllers/ProductController.h \
    src/controllers/POSController.h \
    src/controllers/ReportController.h

FORMS += \
    src/views/LoginWindow.ui \
    src/views/MainWindow.ui \
    src/views/ProductWindow.ui \
    src/views/AddProductDialog.ui \
    src/views/POSWindow.ui \
    src/views/PaymentDialog.ui \
    src/views/ReportWindow.ui

INCLUDEPATH += src

RESOURCES += resources/styles/main.qss
