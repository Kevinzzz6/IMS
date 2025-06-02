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
    bool addSupplier(Supplier& supplier);
    Supplier getSupplier() const;
    void setSupplier(const Supplier& supplier); // 设置供应商数据
    QList<Supplier> getAllSuppliers() const; // ✨ 这是 ProductDialog 需要的
    bool updateSupplier(const Supplier& supplier);
    bool deleteSupplier(int id);
    int getSupplierId(const QString& name) const; // 之前已添加，确保存在

private:
    Ui::SupplierDialog *ui;
    int m_editingSupplierId; // 用于跟踪正在编辑的供应商 ID

};

#endif // SUPPLIERDIALOG_H
