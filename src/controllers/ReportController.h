#ifndef REPORTCONTROLLER_H
#define REPORTCONTROLLER_H

#include <QObject>
#include <QDate>
#include <QVector>
#include "models/Sale.h"
#include "models/Product.h"

struct DailySalesReport {
    QDate date;
    int transactionCount;
    double totalAmount;
    QVector<Sale> sales;
};

struct StockReport {
    QVector<Product> allProducts;
    QVector<Product> lowStockProducts;
};

class ReportController : public QObject {
    Q_OBJECT

public:
    explicit ReportController(QObject *parent = nullptr);
    
    // Rapports de ventes
    DailySalesReport getDailySalesReport(const QDate &date = QDate::currentDate());
    QVector<Sale> getSalesByDateRange(const QDate &startDate, const QDate &endDate);
    Sale getSaleDetails(int saleId);
    
    // Rapports de stock
    StockReport getStockReport();
    QVector<Product> getLowStockProducts(int threshold = 10);
    
    // Génération PDF
    bool generateDailySalesPDF(const DailySalesReport &report, const QString &filePath);
    bool generateStockReportPDF(const StockReport &report, const QString &filePath);

signals:
    void reportGenerated(const QString &filePath);
    void errorOccurred(const QString &error);
};

#endif // REPORTCONTROLLER_H