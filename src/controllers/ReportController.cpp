#include "ReportController.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>

ReportController::ReportController(QObject *parent) : QObject(parent) {}

DailySalesReport ReportController::getDailySalesReport(const QDate &date) {
    DailySalesReport report;
    report.date = date;
    report.transactionCount = 0;
    report.totalAmount = 0.0;
    
    QString dateStr = date.toString("yyyy-MM-dd");
    
    QSqlQuery query;
    query.prepare("SELECT * FROM sales WHERE DATE(created_at) = ?");
    query.addBindValue(dateStr);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to get sales report:" << query.lastError().text();
        return report;
    }
    
    while (query.next()) {
        Sale sale(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("total").toDouble(),
            query.value("cash_received").toDouble(),
            query.value("change_amount").toDouble(),
            query.value("created_at").toDateTime()
        );
        
        report.sales.append(sale);
        report.totalAmount += sale.total;
        report.transactionCount++;
    }
    
    qDebug() << "✅ Daily report:" << report.transactionCount << "transactions," << report.totalAmount << "HTG";
    return report;
}

QVector<Sale> ReportController::getSalesByDateRange(const QDate &startDate, const QDate &endDate) {
    QVector<Sale> sales;
    
    QSqlQuery query;
    query.prepare("SELECT * FROM sales WHERE DATE(created_at) BETWEEN ? AND ? ORDER BY created_at DESC");
    query.addBindValue(startDate.toString("yyyy-MM-dd"));
    query.addBindValue(endDate.toString("yyyy-MM-dd"));
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to get sales by date range:" << query.lastError().text();
        return sales;
    }
    
    while (query.next()) {
        sales.append(Sale(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("total").toDouble(),
            query.value("cash_received").toDouble(),
            query.value("change_amount").toDouble(),
            query.value("created_at").toDateTime()
        ));
    }
    
    return sales;
}

StockReport ReportController::getStockReport() {
    StockReport report;
    
    QSqlQuery query("SELECT * FROM products ORDER BY name");
    
    while (query.next()) {
        Product p(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("barcode").toString(),
            query.value("price").toDouble(),
            query.value("stock").toInt()
        );
        
        report.allProducts.append(p);
        
        if (p.stock < 10) {
            report.lowStockProducts.append(p);
        }
    }
    
    return report;
}

QVector<Product> ReportController::getLowStockProducts(int threshold) {
    QVector<Product> products;
    
    QSqlQuery query;
    query.prepare("SELECT * FROM products WHERE stock < ? ORDER BY stock ASC");
    query.addBindValue(threshold);
    
    if (!query.exec()) {
        qCritical() << "❌ Failed to get low stock products:" << query.lastError().text();
        return products;
    }
    
    while (query.next()) {
        products.append(Product(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("barcode").toString(),
            query.value("price").toDouble(),
            query.value("stock").toInt()
        ));
    }
    
    return products;
}

bool ReportController::generateDailySalesPDF(const DailySalesReport &report, const QString &filePath) {
    QString html = QString(
        "<html><head><style>"
        "body { font-family: Arial; }"
        "h1 { color: #2c3e50; text-align: center; }"
        "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
        "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
        "th { background-color: #3498db; color: white; }"
        ".total { font-weight: bold; font-size: 18px; margin-top: 20px; }"
        "</style></head><body>"
        "<h1>Rapport de Ventes - %1</h1>"
        "<p><strong>Nombre de transactions:</strong> %2</p>"
        "<p><strong>Total des ventes:</strong> %3 HTG</p>"
        "<table>"
        "<tr><th>ID</th><th>Heure</th><th>Montant</th><th>Rendu</th></tr>"
    ).arg(report.date.toString("dd/MM/yyyy"))
     .arg(report.transactionCount)
     .arg(report.totalAmount, 0, 'f', 2);
    
    for (const Sale &sale : report.sales) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3 HTG</td><td>%4 HTG</td></tr>")
                .arg(sale.id)
                .arg(sale.createdAt.toString("HH:mm:ss"))
                .arg(sale.total, 0, 'f', 2)
                .arg(sale.changeAmount, 0, 'f', 2);
    }
    
    html += "</table></body></html>";
    
    QTextDocument document;
    document.setHtml(html);
    
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    
    document.print(&printer);
    
    qDebug() << "✅ PDF generated:" << filePath;
    emit reportGenerated(filePath);
    return true;
}