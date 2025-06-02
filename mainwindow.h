// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel> // <-- 添加此 #include

// 前向声明
class DatabaseManager;
namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDatabaseManager(DatabaseManager *dbManager);

private slots:
    void on_actionExit_triggered();
    void on_addProductButton_clicked();
    // 稍后添加编辑/删除按钮的槽

    void on_editProductButton_clicked();

    void on_deleteProductButton_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager *m_dbManager;   // 指向数据库管理器的指针
    QSqlTableModel *m_productModel; // 产品表视图的模型
    QSqlTableModel *m_categoryModel; // 类别表视图的模型

    void setupProductView(); // 初始化表视图和模型的辅助函数
    void setupCategoryView();
};
#endif // MAINWINDOW_H
