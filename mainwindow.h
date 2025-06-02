#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>           // 用于类别和供应商模型
#include <QSqlRelationalTableModel> // ✨ 用于产品关系模型
#include <QItemSelection>         // ✨ 用于表格选择变化

// 前向声明
class DatabaseManager;
class QSqlRecord; // 可以前向声明，或者在 cpp 中包含 <QSqlRecord>
class QStandardItemModel; // 如果报表模型是成员变量，则需要

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDatabaseManager(DatabaseManager *dbManager);

private slots:
    // 文件菜单
    void on_actionExit_triggered();

    // 产品管理
    void on_addProductButton_clicked();
    void on_editProductButton_clicked();
    void on_deleteProductButton_clicked();
    void on_productSearchLineEdit_textChanged(const QString &text);             // ✨ 新增槽：产品名称搜索
    void on_productCategoryFilterComboBox_currentIndexChanged(int index);   // ✨ 新增槽：产品类别筛选

    // 类别管理
    void on_addCategoryButton_clicked();
    void on_editCategoryButton_clicked();
    void on_deleteCategoryButton_clicked();

    // 供应商管理
    void on_addSupplierButton_clicked();
    void on_editSupplierButton_clicked();
    void on_deleteSupplierButton_clicked();

    // 报表管理
    void on_generateLowStockReportButton_clicked(); // ✨ 新增槽：生成低库存报表

    // ✨ 新增槽：处理产品表格选择变化 (可选，用于启用/禁用按钮)
    void on_productSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void on_clearProductFilterButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager *m_dbManager;

    // 模型
    QSqlRelationalTableModel *m_productRelationalModel; // ✨ 用于产品视图的关系模型
    QSqlTableModel *m_categoryModel;                  // 用于类别视图
    QSqlTableModel *m_supplierModel;                  // 用于供应商视图
    // QStandardItemModel *m_lowStockReportModel; // ✨ 如果低库存报表模型需要作为成员变量

    // 私有辅助函数
    void setupProductView();
    void setupCategoryView();
    void setupSupplierView();
    void populateCategoryFilterComboBox(); // ✨ 新增辅助函数：填充类别筛选框
    void applyProductFilters();            // ✨ 新增辅助函数：应用产品筛选条件
    void updateProductActionButtons();     // ✨ 新增辅助函数：根据选择更新产品按钮状态
};
#endif // MAINWINDOW_H
