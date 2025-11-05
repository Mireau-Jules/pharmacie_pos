QT += core sql printsupport
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_backend

INCLUDEPATH += . database src

# Source du test
SOURCES += test_backend_complete.cpp

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
SOURCES += database/DatabaseManager.cpp
HEADERS += database/DatabaseManager.h

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