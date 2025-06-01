#include "mainwindow.h"
#include "ui_mainwindow.h" // 注意：通常是 ui_mainwindow.h，检查您的文件名
#include "databasemanager.h"
#include "productdialog.h"
#include <QMessageBox>
#include <QSqlError>      // 用于 m_productModel->lastError()
#include <QHeaderView>    // 用于自定义表头外观

// 确保 m_dbManager 是 MainWindow 的成员并已初始化
// void MainWindow::setDatabaseManager(DatabaseManager *dbManager) {
//     m_dbManager = dbManager;
//     if (m_dbManager && m_dbManager->getDatabase().isOpen()) {
//         setupProductView(); // 假设在设置 dbManager 后调用此函数
//         // 您可能还想在此处或 ProductDialog 本身中为 ProductDialog 加载类别/供应商
//     }
// }

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

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

