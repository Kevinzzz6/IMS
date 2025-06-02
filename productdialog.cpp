// productdialog.cpp
#include "productdialog.h"
#include "ui_productdialog.h"
#include "databasemanager.h" // 确保完整包含 DatabaseManager
#include "category.h"
#include "supplier.h"
#include <QComboBox>
#include <QMessageBox> // 用于验证消息
#include <QTextEdit>


ProductDialog::ProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductDialog),
    m_editingProductId(-1) // 初始化为“添加”模式
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

void ProductDialog::setProduct(const Product& product) {
    m_editingProductId = product.id; // 存储 ID
    ui->productNameLineEdit->setText(product.name);
    ui->descriptionTextEdit->setText(product.description);
    ui->purchasePriceSpinBox->setValue(product.purchasePrice);
    ui->sellingPriceSpinBox->setValue(product.sellingPrice);
    ui->stockQuantitySpinBox->setValue(product.stockQuantity);

    // TODO 在阶段 4.3：在 categoryComboBox 和 supplierComboBox 中选择正确的项目
    int catIndex = ui->categoryComboBox->findData(product.categoryId);
    if (catIndex!= -1) {
        ui->categoryComboBox->setCurrentIndex(catIndex);
    } else {
        ui->categoryComboBox->setCurrentIndex(ui->categoryComboBox->findData(-1)); // 选择“无”
    }

    int supIndex = ui->supplierComboBox->findData(product.supplierId);
    if (supIndex!= -1) {
        ui->supplierComboBox->setCurrentIndex(supIndex);
    } else {
        ui->supplierComboBox->setCurrentIndex(ui->supplierComboBox->findData(-1)); // 选择“无”
    }

    setWindowTitle("编辑产品 - ID：" + QString::number(product.id));
}


Product ProductDialog::getProduct() const {
    Product product;
    // product.id = m_currentProduct.id; // 对于编辑很重要，对于添加则是新的
    product.id = m_editingProductId; // 为更新操作设置 ID
    product.name = ui->productNameLineEdit->text().trimmed();
    product.description = ui->descriptionTextEdit->text().trimmed();

    product.purchasePrice = ui->purchasePriceSpinBox->value(); // QDoubleSpinBox 直接给出 double
    product.sellingPrice = ui->sellingPriceSpinBox->value();   // QDoubleSpinBox 直接给出 double

    product.stockQuantity = ui->stockQuantitySpinBox->value(); // QSpinBox 直接给出 int

    product.categoryId = ui->categoryComboBox->currentData().toInt();
    product.supplierId = ui->supplierComboBox->currentData().toInt();
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

void ProductDialog::loadCategoriesAndSuppliers(DatabaseManager* dbManager) {
    if (!dbManager) return;

    ui->categoryComboBox->clear();
    QList<Category> categories = dbManager->getAllCategories();
    ui->categoryComboBox->addItem(tr("无"), -1); // 无类别的选项
    for (const Category& cat : categories) {
        ui->categoryComboBox->addItem(cat.name, cat.id); // 将 ID 存储为项目数据
    }

    ui->supplierComboBox->clear();
    QList<Supplier> suppliers = dbManager->getAllSuppliers();
    ui->supplierComboBox->addItem(tr("无"), -1); // 无供应商的选项
    for (const Supplier& sup : suppliers) {
        ui->supplierComboBox->addItem(sup.name, sup.id); // 将 ID 存储为项目数据
    }
}
