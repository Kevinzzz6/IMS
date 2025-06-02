#ifndef SUPPLIERDIALOG_H
#define SUPPLIERDIALOG_H

#include <QDialog>
#include "supplier.h" // 包含供应商结构体定义

namespace Ui {
class SupplierDialog;
}

class SupplierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupplierDialog(QWidget *parent = nullptr);
    ~SupplierDialog();
    Supplier getSupplier() const;
    void setSupplier(const Supplier& supplier); // 设置供应商数据

private:
    Ui::SupplierDialog *ui;
    int m_editingSupplierId; // 用于跟踪正在编辑的供应商 ID
};

#endif // SUPPLIERDIALOG_H
