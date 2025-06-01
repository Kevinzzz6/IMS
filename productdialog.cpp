// productdialog.cpp
#include "productdialog.h"
#include "ui_productdialog.h"
#include <QMessageBox> // 用于验证消息
#include <QTextEdit>

ProductDialog::ProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加/编辑产品");

    // 可选：如果未通过角色自动连接，则连接 QDialogButtonBox 信号
    // connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ProductDialog::accept);
    // connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ProductDialog::reject);
}

ProductDialog::~ProductDialog() {
    delete ui;
}

Product ProductDialog::getProduct() const {
    Product product;
    // product.id = m_currentProduct.id; // 对于编辑很重要，对于添加则是新的

    product.name = ui->productNameLineEdit->text().trimmed();
    product.description = ui->descriptionTextEdit->text().trimmed();

    product.purchasePrice = ui->purchasePriceSpinBox->value(); // QDoubleSpinBox 直接给出 double
    product.sellingPrice = ui->sellingPriceSpinBox->value();   // QDoubleSpinBox 直接给出 double

    product.stockQuantity = ui->stockQuantitySpinBox->value(); // QSpinBox 直接给出 int

    // 对于 ComboBox：检索存储的 ID（与所选项目关联的数据）
    // 这将在阶段 4.3 中完全实现
    if (ui->categoryComboBox->currentIndex() >= 0) { // 检查是否选择了项目
        product.categoryId = ui->categoryComboBox->currentData().toInt();
    } else {
        product.categoryId = -1; // 或其他表示无类别的指示符
    }

    if (ui->supplierComboBox->currentIndex() >= 0) {
        product.supplierId = ui->supplierComboBox->currentData().toInt();
    } else {
        product.supplierId = -1; // 或其他表示无供应商的指示符
    }

    return product;
}
