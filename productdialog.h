// productdialog.h
#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include "product.h"        // 用于 Product 结构体
#include "databasemanager.h" // 如果需要类型，则前向声明或包含

namespace Ui {
class ProductDialog;
}

class ProductDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

    Product getProduct() const; // 将 UI 字段中的数据检索到 Product 结构体中
    // void setProduct(const Product& product, DatabaseManager* dbManager); // 用于编辑（阶段 3.4）
    // void loadCategoriesAndSuppliers(DatabaseManager* dbManager); // 用于填充组合框（阶段 4.3）

private:
    Ui::ProductDialog *ui;
    // Product m_currentProduct; // 用于在编辑时存储产品 ID
};

#endif // PRODUCTDIALOG_H
