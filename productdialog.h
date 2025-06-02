#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include "product.h"
class DatabaseManager; // 声明如果需要
struct Category;       // 如果在函数签名或成员中使用
struct Supplier;       // 如果在函数签名或成员中使用

namespace Ui {
class ProductDialog;
}

class ProductDialog : public QDialog
{
    Q_OBJECT

public: // ✨ 确保 setProduct 在 public 区域
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

    Product getProduct() const;
    void setProduct(const Product& product); // 声明在这里
    void extracted(QList<Supplier> &suppliers);
    void
    loadCategoriesAndSuppliers(DatabaseManager *dbManager); // (来自阶段 4.3)

private:
    Ui::ProductDialog *ui;
    int m_editingProductId;
};

#endif // PRODUCTDIALOG_H
