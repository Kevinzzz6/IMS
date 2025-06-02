#include "mainwindow.h"
#include <QStandardItemModel> // ✨ 添加这一行
#include <QStandardItem>    // ✨ 添加这一行
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include "productdialog.h"
#include "product.h"         // 用于 Product 结构体
#include "categorydialog.h"  // 用于 CategoryDialog
#include "supplierdialog.h"  // 用于 SupplierDialog
#include "category.h"        // 用于 Category 结构体
// #include "supplier.h"     // 如果 Supplier 结构体也在此处使用，则包含 (通常在 DatabaseManager 或 SupplierDialog 中已包含)
#include <QMessageBox>
#include <QSqlError>         // 用于模型错误处理
#include <QHeaderView>       // 用于自定义表头外观
#include <QSqlRecord>        // 用于访问模型记录
#include <QSqlRelationalTableModel> // ✨ 确保包含 QSqlRelationalTableModel
#include <QSqlRelation>             // ✨ 确保包含 QSqlRelation
#include <QSqlRelationalDelegate>   // ✨ 确保包含 (如果使用表格内编辑)
#include <QDebug>                   // 用于 qDebug() 输出
#include <QApplication>             // 用于 QApplication::quit()

// MainWindow 构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dbManager(nullptr)
    , m_productRelationalModel(nullptr) // ***** 更新点 ***** 初始化 m_productRelationalModel
    , m_categoryModel(nullptr)          // 初始化 m_categoryModel
    , m_supplierModel(nullptr)          // 初始化 m_supplierModel
    // , m_lowStockReportModel(nullptr) // 如果作为成员变量初始化
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("就绪"), 3000);

    // 可以在此处或 setup...View 函数中进行信号槽连接
    // 例如，为产品表格选择变化的连接：
    // connect(ui->productsTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
    //         this, &MainWindow::on_productSelectionChanged);
    // 初始禁用编辑和删除按钮
    // ui->editProductButton->setEnabled(false);
    // ui->deleteProductButton->setEnabled(false);
    // (类似的逻辑也适用于类别和供应商的按钮)
}

MainWindow::~MainWindow()
{
    delete ui;
    // m_productRelationalModel, m_categoryModel, m_supplierModel 作为 this 的子对象，会被 Qt 自动管理删除。
    // 如果它们没有被设置父对象，则需要在这里 delete。
}

void MainWindow::setDatabaseManager(DatabaseManager *dbManager) {
    m_dbManager = dbManager;
    if (m_dbManager && m_dbManager->getDatabase().isOpen()) {
        setupProductView();
        setupCategoryView();
        setupSupplierView();
        populateCategoryFilterComboBox(); // ✨ 新增：在数据库就绪后填充筛选框
    } else {
        QMessageBox::critical(this, "数据库错误", "MainWindow 的数据库管理器未正确初始化。");
    }
}

void MainWindow::setupProductView() {
    if (!m_dbManager) {
        qDebug() << "setupProductView：DatabaseManager 未设置。";
        return;
    }

    if (m_productRelationalModel) {
        delete m_productRelationalModel;
        m_productRelationalModel = nullptr;
    }

    m_productRelationalModel = new QSqlRelationalTableModel(this, m_dbManager->getDatabase());
    m_productRelationalModel->setTable("Products");
    m_productRelationalModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    int categoryColIdx = m_productRelationalModel->fieldIndex("categoryId");
    if (categoryColIdx != -1) {
        m_productRelationalModel->setRelation(categoryColIdx, QSqlRelation("Categories", "id", "name"));
    } else {
        qDebug() << "警告：在 Products 表中未找到 categoryId 列。";
    }

    int supplierColIdx = m_productRelationalModel->fieldIndex("supplierId");
    if (supplierColIdx != -1) {
        m_productRelationalModel->setRelation(supplierColIdx, QSqlRelation("Suppliers", "id", "name"));
    } else {
        qDebug() << "警告：在 Products 表中未找到 supplierId 列。";
    }

    m_productRelationalModel->setHeaderData(m_productRelationalModel->fieldIndex("name"), Qt::Horizontal, tr("产品名称"));
    m_productRelationalModel->setHeaderData(m_productRelationalModel->fieldIndex("description"), Qt::Horizontal, tr("描述"));
    m_productRelationalModel->setHeaderData(m_productRelationalModel->fieldIndex("sellingPrice"), Qt::Horizontal, tr("销售价格"));
    m_productRelationalModel->setHeaderData(m_productRelationalModel->fieldIndex("stockQuantity"), Qt::Horizontal, tr("库存数量"));

    if (categoryColIdx != -1) {
        m_productRelationalModel->setHeaderData(categoryColIdx, Qt::Horizontal, tr("类别"));
    }
    if (supplierColIdx != -1) {
        m_productRelationalModel->setHeaderData(supplierColIdx, Qt::Horizontal, tr("供应商"));
    }

    if (!m_productRelationalModel->select()) {
        qDebug() << "加载产品关系模型时出错：" << m_productRelationalModel->lastError().text();
        QMessageBox::critical(this, "模型错误",
                              "无法从数据库加载产品：" + m_productRelationalModel->lastError().text());
        return;
    }

    ui->productsTableView->setModel(m_productRelationalModel);
    // ui->productsTableView->setItemDelegate(new QSqlRelationalDelegate(ui->productsTableView)); // 如果需要表格内编辑关系字段

    ui->productsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->productsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->productsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->productsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->productsTableView->resizeColumnsToContents();
    ui->productsTableView->hideColumn(m_productRelationalModel->fieldIndex("id"));
    ui->productsTableView->hideColumn(m_productRelationalModel->fieldIndex("purchasePrice"));

    qDebug() << "产品视图(关系模型)设置完成。模型有" << m_productRelationalModel->rowCount() << "行。";
}

void MainWindow::setupCategoryView() {
    if (!m_dbManager) {
        qDebug() << "setupCategoryView：DatabaseManager 未设置。";
        return;
    }
    if(m_categoryModel){ delete m_categoryModel; m_categoryModel = nullptr; } // 清理旧模型

    m_categoryModel = new QSqlTableModel(this, m_dbManager->getDatabase());
    m_categoryModel->setTable("Categories");
    m_categoryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_categoryModel->setHeaderData(m_categoryModel->fieldIndex("name"), Qt::Horizontal, tr("类别名称"));
    if (!m_categoryModel->select()) {
        qDebug() << "加载类别模型时出错：" << m_categoryModel->lastError().text();
        QMessageBox::critical(this, "模型错误", "无法从数据库加载类别：" + m_categoryModel->lastError().text());
        return;
    }
    ui->categoriesTableView->setModel(m_categoryModel);
    ui->categoriesTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categoriesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->categoriesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->categoriesTableView->resizeColumnsToContents();
    ui->categoriesTableView->hideColumn(m_categoryModel->fieldIndex("id"));
    qDebug() << "类别视图设置完成。模型有" << m_categoryModel->rowCount() << "行。";
}

void MainWindow::setupSupplierView() {
    if (!m_dbManager) {
        qDebug() << "setupSupplierView：DatabaseManager 未设置。";
        return;
    }
    if(m_supplierModel){ delete m_supplierModel; m_supplierModel = nullptr; } // 清理旧模型

    m_supplierModel = new QSqlTableModel(this, m_dbManager->getDatabase());
    m_supplierModel->setTable("Suppliers");
    m_supplierModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_supplierModel->setHeaderData(m_supplierModel->fieldIndex("name"), Qt::Horizontal, tr("供应商名称"));
    m_supplierModel->setHeaderData(m_supplierModel->fieldIndex("contactDetails"), Qt::Horizontal, tr("联系信息"));
    if (!m_supplierModel->select()) {
        qDebug() << "加载供应商模型时出错：" << m_supplierModel->lastError().text();
        QMessageBox::critical(this, "模型错误", "无法从数据库加载供应商：" + m_supplierModel->lastError().text());
        return;
    }
    ui->suppliersTableView->setModel(m_supplierModel);
    ui->suppliersTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->suppliersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->suppliersTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->suppliersTableView->horizontalHeader()->setStretchLastSection(true);
    ui->suppliersTableView->resizeColumnsToContents();
    ui->suppliersTableView->hideColumn(m_supplierModel->fieldIndex("id"));
    qDebug() << "供应商视图设置完成。模型有" << m_supplierModel->rowCount() << "行。";
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

// --- 产品管理槽函数 ---
void MainWindow::on_addProductButton_clicked() {
    if (!m_dbManager) {
        QMessageBox::critical(this, "错误", "数据库管理器未初始化。");
        return;
    }

    ProductDialog dialog(this);
    dialog.loadCategoriesAndSuppliers(m_dbManager); // 确保在显示对话框前加载

    if (dialog.exec() == QDialog::Accepted) {
        Product product = dialog.getProduct();
        if (product.name.isEmpty() || product.sellingPrice <= 0) {
            QMessageBox::warning(this, "输入错误", "产品名称不能为空且销售价格必须为正数。");
            return;
        }
        if (m_dbManager->addProduct(product)) {
            QMessageBox::information(this, "成功", "产品添加成功，ID 为：" + QString::number(product.id));
            if (m_productRelationalModel) { // ***** 更新点 *****
                m_productRelationalModel->select(); // 刷新 QSqlRelationalTableModel
            }
        } else {
            QMessageBox::warning(this, "数据库错误", "无法将产品添加到数据库。");
        }
    } else {
        qDebug() << "添加产品对话框已取消。";
    }
}

void MainWindow::on_editProductButton_clicked() {
    if (!m_dbManager || !m_productRelationalModel) { // ***** 更新点 *****
        QMessageBox::critical(this, "错误", "系统未正确初始化 (产品模型无效)。");
        return;
    }

    QModelIndex currentIndex = ui->productsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要编辑的产品。");
        return;
    }

    // 从关系模型中获取记录。'id' 字段是 Products 表的原始字段，可以直接获取。
    int productId = m_productRelationalModel->record(currentIndex.row()).value("id").toInt(); // ***** 更新点 *****
    if (productId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的产品 ID。");
        return;
    }

    Product productToEdit = m_dbManager->getProductById(productId);
    if (productToEdit.id == -1) {
        QMessageBox::warning(this, "错误", "无法检索要编辑的产品详细信息。");
        return;
    }

    ProductDialog dialog(this);
    dialog.loadCategoriesAndSuppliers(m_dbManager); // 加载下拉框数据
    dialog.setProduct(productToEdit);               // 预填充对话框

    if (dialog.exec() == QDialog::Accepted) {
        Product updatedProduct = dialog.getProduct();
        updatedProduct.id = productId; // 确保 ID 正确
        if (updatedProduct.name.isEmpty() || updatedProduct.sellingPrice <= 0) {
            QMessageBox::warning(this, "输入错误", "产品名称不能为空且销售价格必须为正数。");
            return;
        }
        if (m_dbManager->updateProduct(updatedProduct)) {
            QMessageBox::information(this, "成功", "产品更新成功。");
            m_productRelationalModel->select(); // ***** 更新点 ***** 刷新表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法更新产品。");
        }
    }
}

void MainWindow::on_deleteProductButton_clicked() {
    if (!m_dbManager || !m_productRelationalModel) { // ***** 更新点 *****
        QMessageBox::critical(this, "错误", "系统未正确初始化 (产品模型无效)。");
        return;
    }

    QModelIndex currentIndex = ui->productsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要删除的产品。");
        return;
    }

    int productId = m_productRelationalModel->record(currentIndex.row()).value("id").toInt(); // ***** 更新点 *****
    if (productId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的产品 ID 进行删除。");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("您确定要删除产品 ID %1 吗？此操作不可撤销。").arg(productId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dbManager->deleteProduct(productId)) {
            QMessageBox::information(this, "成功", "产品删除成功。");
            m_productRelationalModel->select(); // ***** 更新点 ***** 刷新表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法删除产品。");
        }
    }
}

// --- 类别管理槽函数 ---
void MainWindow::on_addCategoryButton_clicked()
{
    if (!m_dbManager) {
        QMessageBox::critical(this, "错误", "数据库管理器未初始化。");
        return;
    }

    CategoryDialog dialog(this);
    dialog.setWindowTitle("添加类别");
    if (dialog.exec() == QDialog::Accepted) {
        Category newCategory = dialog.getCategory();
        if (newCategory.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "类别名称不能为空。");
            return;
        }
        if (m_dbManager->addCategory(newCategory)) {
            QMessageBox::information(this, "成功", "类别添加成功，ID 为：" + QString::number(newCategory.id));
            m_categoryModel->select(); // 刷新类别列表视图
            populateCategoryFilterComboBox(); // ✨ 新增：添加类别后更新产品筛选框
        } else {
            QMessageBox::warning(this, "数据库错误", "无法添加类别到数据库，可能名称已存在。");
        }
    } else {
        qDebug() << "添加类别对话框已取消。";
    }
}

void MainWindow::on_editCategoryButton_clicked()
{
    if (!m_dbManager || !m_categoryModel) {
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    QModelIndex currentIndex = ui->categoriesTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要编辑的类别。");
        return;
    }

    int categoryId = m_categoryModel->record(currentIndex.row()).value("id").toInt();
    if (categoryId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的类别 ID。");
        return;
    }
    if (categoryId == 1 && m_categoryModel->record(currentIndex.row()).value("name").toString() == "未分类") { // 假设ID为1的是“未分类”
        QMessageBox::warning(this, "操作受限", "不能编辑默认的“未分类”类别。");
        return;
    }

    Category categoryToEdit = m_dbManager->getCategoryById(categoryId);
    if (categoryToEdit.id == -1) {
        QMessageBox::warning(this, "错误", "无法检索要编辑的类别详细信息。");
        return;
    }

    CategoryDialog dialog(this);
    dialog.setWindowTitle("编辑类别");
    dialog.setCategory(categoryToEdit);

    if (dialog.exec() == QDialog::Accepted) {
        Category updatedCategory = dialog.getCategory();
        updatedCategory.id = categoryId;
        if (updatedCategory.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "类别名称不能为空。");
            return;
        }
        if (m_dbManager->updateCategory(updatedCategory)) {
            QMessageBox::information(this, "成功", "类别更新成功。");
            m_categoryModel->select(); // 刷新类别列表视图
            m_productRelationalModel->select(); // ✨ 新增：编辑类别后刷新产品视图以更新关系显示
            populateCategoryFilterComboBox();   // ✨ 新增：编辑类别后更新产品筛选框
        } else {
            QMessageBox::warning(this, "数据库错误", "无法更新类别，可能名称与现有类别冲突。");
        }
    }
}

void MainWindow::on_deleteCategoryButton_clicked()
{
    if (!m_dbManager || !m_categoryModel) {
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    QModelIndex currentIndex = ui->categoriesTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要删除的类别。");
        return;
    }

    int categoryId = m_categoryModel->record(currentIndex.row()).value("id").toInt();
    if (categoryId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的类别 ID 进行删除。");
        return;
    }
    if (categoryId == 1 && m_categoryModel->record(currentIndex.row()).value("name").toString() == "未分类") { // 假设ID为1的是“未分类”
        QMessageBox::warning(this, "操作受限", "不能删除默认的“未分类”类别。");
        return;
    }


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("您确定要删除类别 ID %1 吗？关联产品的类别将设为“未分类”(如果数据库设置了 ON DELETE SET NULL)。").arg(categoryId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 注意：如果 Products 表中的 categoryId 有外键约束且设置为 ON DELETE RESTRICT 或 ON DELETE NO ACTION，
        // 并且有产品正在使用此类别，则删除会失败。
        // 当前设计是 ON DELETE SET NULL，所以产品会变为未分类。
        if (m_dbManager->deleteCategory(categoryId)) {
            QMessageBox::information(this, "成功", "类别删除成功。");
            m_categoryModel->select(); // 刷新类别列表视图
            m_productRelationalModel->select(); // ✨ 新增：删除类别后刷新产品视图
            populateCategoryFilterComboBox();   // ✨ 新增：删除类别后更新产品筛选框
        } else {
            QMessageBox::warning(this, "数据库错误", "无法删除类别。请检查是否有产品仍在使用此类别（如果外键约束阻止删除）。");
        }
    }
}

// --- 供应商管理槽函数 ---
void MainWindow::on_addSupplierButton_clicked()
{
    if (!m_dbManager) {
        QMessageBox::critical(this, "错误", "数据库管理器未初始化。");
        return;
    }

    SupplierDialog dialog(this);
    dialog.setWindowTitle("添加供应商");
    if (dialog.exec() == QDialog::Accepted) {
        Supplier newSupplier = dialog.getSupplier();
        if (newSupplier.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "供应商名称不能为空。");
            return;
        }
        if (m_dbManager->addSupplier(newSupplier)) {
            QMessageBox::information(this, "成功", "供应商添加成功，ID 为：" + QString::number(newSupplier.id));
            m_supplierModel->select(); // 刷新供应商列表视图
            // ✨ 可选：如果产品对话框的供应商下拉框是动态加载的，这里也需要通知更新
            // populateCategoryFilterComboBox(); // 这个是分类的，供应商类似
        } else {
            QMessageBox::warning(this, "数据库错误", "无法添加供应商到数据库，可能名称已存在。");
        }
    } else {
        qDebug() << "添加供应商对话框已取消。";
    }
}

void MainWindow::on_editSupplierButton_clicked()
{
    if (!m_dbManager || !m_supplierModel) {
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    QModelIndex currentIndex = ui->suppliersTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要编辑的供应商。");
        return;
    }

    int supplierId = m_supplierModel->record(currentIndex.row()).value("id").toInt();
    if (supplierId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的供应商 ID。");
        return;
    }
    if (supplierId == 1 && m_supplierModel->record(currentIndex.row()).value("name").toString() == "默认供应商") { // 假设ID为1的是“默认供应商”
        QMessageBox::warning(this, "操作受限", "不能编辑默认的“默认供应商”。");
        return;
    }


    Supplier supplierToEdit = m_dbManager->getSupplierById(supplierId);
    if (supplierToEdit.id == -1) {
        QMessageBox::warning(this, "错误", "无法检索要编辑的供应商详细信息。");
        return;
    }

    SupplierDialog dialog(this);
    dialog.setWindowTitle("编辑供应商");
    dialog.setSupplier(supplierToEdit);

    if (dialog.exec() == QDialog::Accepted) {
        Supplier updatedSupplier = dialog.getSupplier();
        updatedSupplier.id = supplierId;
        if (updatedSupplier.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "供应商名称不能为空。");
            return;
        }
        if (m_dbManager->updateSupplier(updatedSupplier)) {
            QMessageBox::information(this, "成功", "供应商更新成功。");
            m_supplierModel->select(); // 刷新供应商列表视图
            m_productRelationalModel->select(); // ✨ 新增：编辑供应商后刷新产品视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法更新供应商，可能名称与现有供应商冲突。");
        }
    }
}

void MainWindow::on_deleteSupplierButton_clicked()
{
    if (!m_dbManager || !m_supplierModel) {
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    QModelIndex currentIndex = ui->suppliersTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要删除的供应商。");
        return;
    }

    int supplierId = m_supplierModel->record(currentIndex.row()).value("id").toInt();
    if (supplierId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的供应商 ID 进行删除。");
        return;
    }
    if (supplierId == 1 && m_supplierModel->record(currentIndex.row()).value("name").toString() == "默认供应商") { // 假设ID为1的是“默认供应商”
        QMessageBox::warning(this, "操作受限", "不能删除默认的“默认供应商”。");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("您确定要删除供应商 ID %1 吗？关联产品的供应商将设为“默认供应商”(如果数据库设置了 ON DELETE SET NULL)。").arg(supplierId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dbManager->deleteSupplier(supplierId)) {
            QMessageBox::information(this, "成功", "供应商删除成功。");
            m_supplierModel->select(); // 刷新供应商列表视图
            m_productRelationalModel->select(); // ✨ 新增：删除供应商后刷新产品视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法删除供应商。");
        }
    }
}


// ✨ 新增：用于产品筛选框的槽函数和辅助函数 (假设已在 .h 中声明)
// --- 产品筛选槽函数和辅助函数 ---
void MainWindow::populateCategoryFilterComboBox() { // ✨ 实现
    if (!m_dbManager) return;
    QVariant currentData = ui->productCategoryFilterComboBox->currentData();
    ui->productCategoryFilterComboBox->clear();
    ui->productCategoryFilterComboBox->addItem(tr("所有类别"), -1);
    QList<Category> categories = m_dbManager->getAllCategories();
    for (const Category& cat : categories) {
        ui->productCategoryFilterComboBox->addItem(cat.name, cat.id);
    }
    int indexToRestore = ui->productCategoryFilterComboBox->findData(currentData);
    ui->productCategoryFilterComboBox->setCurrentIndex(indexToRestore != -1 ? indexToRestore : 0);
}

void MainWindow::applyProductFilters() { // ✨ 实现
    if (!m_productRelationalModel) {
        qDebug() << "产品关系模型未初始化，无法应用筛选。";
        return;
    }
    QString searchText = ui->productSearchLineEdit->text().trimmed();
    int categoryId = ui->productCategoryFilterComboBox->currentData().toInt();
    QStringList filters;
    if (!searchText.isEmpty()) {
        filters.append(QString("Products.name LIKE '%%1%'").arg(searchText.replace("'", "''")));
    }
    if (categoryId != -1) {
        filters.append(QString("Products.categoryId = %1").arg(categoryId));
    }
    QString combinedFilter = filters.join(" AND ");
    m_productRelationalModel->setFilter(combinedFilter);
    if (!m_productRelationalModel->select()) {
        qDebug() << "应用产品筛选器时出错：" << m_productRelationalModel->lastError().text();
        QMessageBox::warning(this, "筛选错误", "应用筛选条件时发生错误：" + m_productRelationalModel->lastError().text());
    } else {
        qDebug() << "产品筛选器已应用：" << combinedFilter << "；找到 " << m_productRelationalModel->rowCount() << " 行。";
    }
    updateProductActionButtons(); // ✨ 更新按钮状态
}

void MainWindow::on_productSearchLineEdit_textChanged(const QString &text) { // ✨ 实现
    Q_UNUSED(text);
    applyProductFilters();
}

void MainWindow::on_productCategoryFilterComboBox_currentIndexChanged(int index) { // ✨ 实现
    Q_UNUSED(index);
    applyProductFilters();
}

// --- 低库存报表槽函数 ---
void MainWindow::on_generateLowStockReportButton_clicked() { // ✨ 实现
    if (!m_dbManager) {
        QMessageBox::warning(this, "错误", "数据库未初始化。");
        return;
    }
    int threshold = ui->lowStockThresholdSpinBox->value();
    QList<Product> lowStockProducts = m_dbManager->getLowStockProducts(threshold);

    // 清理旧的报表模型（如果存在）
    if (ui->lowStockTableView->model()) {
        delete ui->lowStockTableView->model();
        ui->lowStockTableView->setModel(nullptr);
    }

    if (lowStockProducts.isEmpty()) {
        QMessageBox::information(this, "低库存报表", "没有找到库存低于或等于 " + QString::number(threshold) + " 的产品。");
        return;
    }

    QStandardItemModel *reportModel = new QStandardItemModel(lowStockProducts.size(), 4, ui->lowStockTableView); // ✨ 设置父对象
    reportModel->setHorizontalHeaderLabels({tr("ID"), tr("产品名称"), tr("当前库存"), tr("售价")});
    for (int row = 0; row < lowStockProducts.size(); ++row) {
        const Product& product = lowStockProducts.at(row);
        reportModel->setItem(row, 0, new QStandardItem(QString::number(product.id)));
        reportModel->setItem(row, 1, new QStandardItem(product.name));
        reportModel->setItem(row, 2, new QStandardItem(QString::number(product.stockQuantity)));
        reportModel->setItem(row, 3, new QStandardItem(QString::number(product.sellingPrice)));
    }
    ui->lowStockTableView->setModel(reportModel);
    ui->lowStockTableView->resizeColumnsToContents();
    ui->lowStockTableView->horizontalHeader()->setStretchLastSection(true);
    ui->lowStockTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// --- 产品表格选择变化槽函数 ---
void MainWindow::on_productSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) // ✨ 实现
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    updateProductActionButtons();
}

// --- 更新产品操作按钮状态的辅助函数 ---
void MainWindow::updateProductActionButtons() // ✨ 实现
{
    bool hasSelection = false; // 默认值
    if (ui->productsTableView && ui->productsTableView->selectionModel()) { // ✨ 增加空指针检查
        hasSelection = ui->productsTableView->selectionModel()->hasSelection();
    }

    if (ui->editProductButton) { // ✨ 增加空指针检查
        ui->editProductButton->setEnabled(hasSelection); // ✨ 使用 hasSelection
    }
    if (ui->deleteProductButton) { // ✨ 增加空指针检查
        ui->deleteProductButton->setEnabled(hasSelection); // ✨ 使用 hasSelection
    }
}

void MainWindow::on_clearProductFilterButton_clicked()
{
    ui->productSearchLineEdit->clear(); // 清除搜索框
    ui->productCategoryFilterComboBox->setCurrentIndex(0); // 重置类别筛选框到“所有类别”
    applyProductFilters(); // 应用清除后的筛选条件
    updateProductActionButtons(); // 更新按钮状态
    qDebug() << "产品筛选已清除，所有产品将重新显示。";
    statusBar()->showMessage(tr("产品筛选已清除，所有产品将重新显示。"), 3000); // 更新状态栏消息
}

