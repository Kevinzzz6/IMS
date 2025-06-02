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
    if (!dbManager) {
        qDebug() << "ProductDialog 错误：传递的 DatabaseManager 为空。";
        return;
    }

    // --- 填充类别组合框 ---
    ui->categoryComboBox->clear(); // 清除旧项目
    ui->categoryComboBox->addItem(tr("无类别"), QVariant(-1)); // 添加一个“无”或“请选择”的选项，其关联数据为-1
    QList<Category> categories = dbManager->getAllCategories();
    if (categories.isEmpty() && !dbManager->getDatabase().isOpen()) { // 额外检查
        qDebug() << "ProductDialog 警告：无法加载类别，数据库可能未正确打开或类别表为空。";
    }
    for (const Category& cat : std::as_const(categories)) {
        ui->categoryComboBox->addItem(cat.name, QVariant(cat.id)); // 显示名称，存储 ID
    }

    // --- 填充供应商组合框 ---
    ui->supplierComboBox->clear(); // 清除旧项目
    ui->supplierComboBox->addItem(tr("无供应商"), QVariant(-1)); // 添加一个“无”或“请选择”的选项
    QList<Supplier> suppliers = dbManager->getAllSuppliers(); // ✨ 这里调用了 getAllSuppliers
    if (suppliers.isEmpty() && !dbManager->getDatabase().isOpen()) { // 额外检查
        qDebug() << "ProductDialog 警告：无法加载供应商，数据库可能未正确打开或供应商表为空。";
    }
    for (const Supplier& sup : std::as_const(suppliers)) {
        ui->supplierComboBox->addItem(sup.name, QVariant(sup.id)); // 显示名称，存储 ID
    }
}
