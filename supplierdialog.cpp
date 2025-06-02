#include "supplierdialog.h"
#include "ui_supplierdialog.h"

SupplierDialog::SupplierDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SupplierDialog)
{
    ui->setupUi(this);
}

SupplierDialog::~SupplierDialog()
{
    delete ui;
}

void SupplierDialog::setSupplier(const Supplier& supplier)
{
    m_editingSupplierId = supplier.id; // 存储供应商 ID
    ui->supplierNameLineEdit->setText(supplier.name); // 设置供应商名称
    ui->contactDetailsTextEdit->setText(supplier.contactDetails); // 设置联系信息
}

Supplier SupplierDialog::getSupplier() const
{
    Supplier supplier;
    supplier.id = m_editingSupplierId; // 获取供应商 ID
    supplier.name = ui->supplierNameLineEdit->text().trimmed(); // 获取供应商名称
    supplier.contactDetails = ui->contactDetailsTextEdit->toPlainText().trimmed(); // 获取联系信息
    return supplier;
}
