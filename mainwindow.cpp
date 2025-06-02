#include "mainwindow.h"
#include "ui_mainwindow.h" // 注意：通常是 ui_mainwindow.h，检查您的文件名
#include "databasemanager.h"
#include "productdialog.h"
#include "product.h"       // 用于 Product 结构体
#include "categorydialog.h" // 用于 CategoryDialog
#include <QMessageBox>
#include <QSqlError>      // 用于 m_productModel->lastError()
#include <QHeaderView>    // 用于自定义表头外观
#include <QSqlRecord>   // 用于访问模型记录

// 确保 m_dbManager 是 MainWindow 的成员并已初始化
// void MainWindow::setDatabaseManager(DatabaseManager *dbManager) {
//     m_dbManager = dbManager;
//     if (m_dbManager && m_dbManager->getDatabase().isOpen()) {
//         setupProductView(); // 假设在设置 dbManager 后调用此函数
//         // 您可能还想在此处或 ProductDialog 本身中为 ProductDialog 加载类别/供应商
//     }
// }


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dbManager(nullptr)      // 将指针成员初始化为 nullptr
    , m_productModel(nullptr)
{
    ui->setupUi(this);
    // addProductButton 的连接可能通过 on_... 命名约定自动连接
    // connect(ui->addProductButton, &QPushButton::clicked, this, &MainWindow::on_addProductButton_clicked);
    statusBar()->showMessage(tr("就绪"), 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDatabaseManager(DatabaseManager *dbManager) {
    m_dbManager = dbManager;
    // 一旦数据库管理器可用，就设置依赖于它的视图
    if (m_dbManager && m_dbManager->getDatabase().isOpen()) {
        setupProductView();
        setupCategoryView();
    } else {
        QMessageBox::critical(this, "数据库错误", "MainWindow 的数据库管理器未正确初始化。");
    }
}

void MainWindow::setupProductView() {
    if (!m_dbManager) {
        qDebug() << "setupProductView：DatabaseManager 未设置。";
        return;
    }

    // 创建 QSqlTableModel 实例
    // 'this' 参数将 MainWindow 设置为父对象以进行内存管理。
    // 第二个参数是要使用的 QSqlDatabase 对象。
    m_productModel = new QSqlTableModel(this, m_dbManager->getDatabase());

    m_productModel->setTable("Products"); // 指定数据库表

    // 设置编辑策略。OnManualSubmit 表示更改将被缓存，直到调用 submitAll()
    // 或在模型销毁时（如果未提交/恢复）自动提交。
    // 对于最初显示数据，以及稍后通过对话框添加编辑/删除，
    // 这是可以的。如果需要直接编辑表格，则可以使用 OnFieldChange。
    m_productModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

        // 为列设置用户友好的表头名称。
        // 使用 fieldIndex() 按名称获取列索引，使其对模式更改具有鲁棒性。
        m_productModel->setHeaderData(m_productModel->fieldIndex("name"), Qt::Horizontal, tr("产品名称"));
    m_productModel->setHeaderData(m_productModel->fieldIndex("description"), Qt::Horizontal, tr("描述"));
    m_productModel->setHeaderData(m_productModel->fieldIndex("sellingPrice"), Qt::Horizontal, tr("销售价格"));
    m_productModel->setHeaderData(m_productModel->fieldIndex("stockQuantity"), Qt::Horizontal, tr("库存数量"));
    // 如果稍后使用 QSqlRelationalTableModel，则根据需要为其他相关列（如类别或供应商）添加更多表头

    // 使用“Products”表中的数据填充模型。
    // 如果发生错误，select() 返回 false。
    if (!m_productModel->select()) {
        qDebug() << "加载产品模型时出错：" << m_productModel->lastError().text();
        QMessageBox::critical(this, "模型错误",
                              "无法从数据库加载产品：" + m_productModel->lastError().text());
        return;
    }

    // 为 QTableView 设置模型
    ui->productsTableView->setModel(m_productModel);

    // --- QTableView 的 UI 自定义 ---
    ui->productsTableView->setSelectionMode(QAbstractItemView::SingleSelection); // 只允许选择单行
    ui->productsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选择整行
    ui->productsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 暂时禁用表格中的直接编辑
        // 编辑将通过 ProductDialog 完成。
    ui->productsTableView->horizontalHeader()->setStretchLastSection(true); // 使最后一列拉伸以填充可用宽度
    ui->productsTableView->resizeColumnsToContents(); // 根据内容调整列宽

    // 隐藏不用于用户直接查看的列（例如，ID、内部键）
    // 这些列可能以编程方式使用，但不显示。
    ui->productsTableView->hideColumn(m_productModel->fieldIndex("id"));
    ui->productsTableView->hideColumn(m_productModel->fieldIndex("purchasePrice"));
    ui->productsTableView->hideColumn(m_productModel->fieldIndex("categoryId"));
    ui->productsTableView->hideColumn(m_productModel->fieldIndex("supplierId"));

    qDebug() << "产品视图设置完成。模型有" << m_productModel->rowCount() << "行。";
}

// 在 MainWindow::on_addProductButton_clicked() 中，成功添加产品后：
// if (m_dbManager->addProduct(product)) {
//    ...
//     if (m_productModel) {
//         m_productModel->select(); // 刷新表视图
//     }
// }

void MainWindow::setupCategoryView() {
    if (!m_dbManager) {
        qDebug() << "setupCategoryView：DatabaseManager 未设置。";
        return;
    }

    // 创建 QSqlTableModel 实例
    m_categoryModel = new QSqlTableModel(this, m_dbManager->getDatabase());
    m_categoryModel->setTable("Categories");
    m_categoryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置表头数据
    m_categoryModel->setHeaderData(m_categoryModel->fieldIndex("name"), Qt::Horizontal, tr("类别名称"));

    // 填充模型
    if (!m_categoryModel->select()) {
        qDebug() << "加载类别模型时出错：" << m_categoryModel->lastError().text();
        QMessageBox::critical(this, "模型错误",
                              "无法从数据库加载类别：" + m_categoryModel->lastError().text());
        return;
    }

    // 设置 QTableView 模型
    ui->categoriesTableView->setModel(m_categoryModel);
    ui->categoriesTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categoriesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->categoriesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->categoriesTableView->resizeColumnsToContents();

    // 隐藏不需要的列（例如 ID）
    ui->categoriesTableView->hideColumn(m_categoryModel->fieldIndex("id"));

    qDebug() << "类别视图设置完成。模型有" << m_categoryModel->rowCount() << "行。";
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_addProductButton_clicked() {
    if (!m_dbManager) {
        QMessageBox::critical(this, "错误", "数据库管理器未初始化。");
        return;
    }

    ProductDialog dialog(this); // 创建对话框，'this' (MainWindow) 是其父对象
        // 父子关系有助于模态和内存管理。

    // TODO 在阶段 4.3：在 ProductDialog 中填充 categoryComboBox 和 supplierComboBox
    // dialog.loadCategoriesAndSuppliers(m_dbManager);

    // 以模态方式执行对话框。exec() 返回 QDialog::Accepted 或 QDialog::Rejected。
    if (dialog.exec() == QDialog::Accepted) {
        // 用户点击了“确定”
        Product product = dialog.getProduct(); // 从对话框检索数据

        // 基本验证示例（可以在 ProductDialog 本身中进行更广泛的验证）
        if (product.name.isEmpty() || product.sellingPrice <= 0) {
            QMessageBox::warning(this, "输入错误", "产品名称不能为空且销售价格必须为正数。");
            return;
        }

        if (m_dbManager->addProduct(product)) { // product.id 将由 addProduct 更新
            QMessageBox::information(this, "成功", "产品添加成功，ID 为：" + QString::number(product.id));
            // TODO：刷新产品列表视图（例如，m_productModel->select();）
            if (m_productModel) { // 检查模型是否存在
                m_productModel->select(); // 刷新 QSqlTableModel
            }
        } else {
            QMessageBox::warning(this, "数据库错误", "无法将产品添加到数据库。");
        }
    } else {
        // 用户点击了“取消”或关闭了对话框
        qDebug() << "添加产品对话框已取消。";
    }
}

void MainWindow::on_editProductButton_clicked() {
    if (!m_dbManager || !m_productModel) { // 注意：这里应该是 m_productModel 或 m_productRelationalModel
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    // 从 QTableView 获取当前选择
    QModelIndex currentIndex = ui->productsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要编辑的产品。");
        return;
    }

    // 从模型中的选定行检索产品 ID。
    // 'id' 列已隐藏，但其数据仍在模型中。
    int productId = m_productModel->record(currentIndex.row()).value("id").toInt(); // 或 m_productRelationalModel
    if (productId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的产品 ID。");
        return;
    }

    // 使用 DatabaseManager 从数据库获取完整的产品详细信息
    Product productToEdit = m_dbManager->getProductById(productId);
    if (productToEdit.id == -1) { // 假设如果未找到，getProductById 返回 id 为 -1 的产品
        QMessageBox::warning(this, "错误", "无法检索要编辑的产品详细信息。");
        return;
    }

    ProductDialog dialog(this);
    // TODO 在阶段 4.3：dialog.loadCategoriesAndSuppliers(m_dbManager);
    dialog.setProduct(productToEdit); // 使用现有产品数据预填充对话框

    if (dialog.exec() == QDialog::Accepted) {
        Product updatedProduct = dialog.getProduct();
        updatedProduct.id = productId; // 确保为更新维护 ID

        if (updatedProduct.name.isEmpty() || updatedProduct.sellingPrice <= 0) {
            QMessageBox::warning(this, "输入错误", "产品名称不能为空且销售价格必须为正数。");
            return;
        }

        if (m_dbManager->updateProduct(updatedProduct)) {
            QMessageBox::information(this, "成功", "产品更新成功。");
            m_productModel->select(); // 或 m_productRelationalModel->select(); 刷新表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法更新产品。");
        }
    }
}


void MainWindow::on_deleteProductButton_clicked() {
    if (!m_dbManager || !m_productModel) { // 注意：这里应该是 m_productModel 或 m_productRelationalModel
        QMessageBox::critical(this, "错误", "系统未正确初始化。");
        return;
    }

    QModelIndex currentIndex = ui->productsTableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::information(this, "需要选择", "请选择要删除的产品。");
        return;
    }

    int productId = m_productModel->record(currentIndex.row()).value("id").toInt(); // 或 m_productRelationalModel
    if (productId <= 0) {
        QMessageBox::critical(this, "错误", "选择了无效的产品 ID 进行删除。");
        return;
    }

    // 向用户确认删除
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("您确定要删除产品 ID %1 吗？").arg(productId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dbManager->deleteProduct(productId)) {
            QMessageBox::information(this, "成功", "产品删除成功。");
            m_productModel->select(); // 或 m_productRelationalModel->select(); 刷新表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法删除产品。");
        }
    }
}


void MainWindow::on_addCategoryButton_clicked()
{
    if (!m_dbManager) {
        QMessageBox::critical(this, "错误", "数据库管理器未初始化。");
        return;
    }

    CategoryDialog dialog(this); // 创建类别对话框
    dialog.setWindowTitle("添加类别"); // 设置对话框标题
    if (dialog.exec() == QDialog::Accepted) { // 以模态方式执行对话框
        Category newCategory = dialog.getCategory(); // 获取用户输入的类别数据

        if (newCategory.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "类别名称不能为空。");
            return;
        }

        if (m_dbManager->addCategory(newCategory)) { // 添加类别到数据库
            QMessageBox::information(this, "成功", "类别添加成功，ID 为：" + QString::number(newCategory.id));
            m_categoryModel->select(); // 刷新类别列表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法添加类别到数据库。");
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

    // 使用 DatabaseManager 从数据库获取完整的类别详细信息
    Category categoryToEdit = m_dbManager->getCategoryById(categoryId);
    if (categoryToEdit.id == -1) { // 假设如果未找到，getCategoryById 返回 id 为 -1 的类别
        QMessageBox::warning(this, "错误", "无法检索要编辑的类别详细信息。");
        return;
    }

    CategoryDialog dialog(this);
    dialog.setWindowTitle("编辑类别"); // 设置对话框标题
    dialog.setCategory(categoryToEdit); // 使用现有类别数据预填充对话框

    if (dialog.exec() == QDialog::Accepted) {
        Category updatedCategory = dialog.getCategory();
        updatedCategory.id = categoryId; // 确保为更新维护 ID

        if (updatedCategory.name.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "类别名称不能为空。");
            return;
        }

        if (m_dbManager->updateCategory(updatedCategory)) {
            QMessageBox::information(this, "成功", "类别更新成功。");
            m_categoryModel->select(); // 刷新类别列表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法更新类别。");
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

    // 向用户确认删除
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("您确定要删除类别 ID %1 吗？").arg(categoryId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dbManager->deleteCategory(categoryId)) {
            QMessageBox::information(this, "成功", "类别删除成功。");
            m_categoryModel->select(); // 刷新类别列表视图
        } else {
            QMessageBox::warning(this, "数据库错误", "无法删除类别。");
        }
    }
}

