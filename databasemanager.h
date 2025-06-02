#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase> // 用于数据库连接
#include <QString>      // 用于文件路径和查询
#include <QSqlQuery>    // 用于执行 SQL 查询
#include <QSqlError>    // 用于错误处理
#include <QDebug>       // 用于打印调试消息
#include <QDir>         // 用于目录操作
#include <QStandardPaths> // 用于平台无关的路径
#include "product.h"    // 包含数据结构头文件
#include "category.h"
#include "supplier.h"
#include <QList>        // 用于返回对象列表
#include <QVariant>     // 用于 lastInsertId()

// 前向声明数据结构以避免循环依赖（如果它们包含 DatabaseManager）
// （此处并非严格需要，因为 DBManager 的头文件尚未使用它们）
// struct Product;
// struct Category;
// struct Supplier;

class DatabaseManager {
public:
    // 构造函数接受数据库文件的路径
    explicit DatabaseManager(const QString& path);
    ~DatabaseManager();

    bool openDatabase();    // 打开（如果不存在则创建）数据库的方法
    void closeDatabase();   // 关闭数据库连接的方法
    bool createTables();    // 如果表不存在则创建必要表的方法

    // 数据库对象的 getter，对 QSqlTableModel 很有用
    QSqlDatabase getDatabase() const;
    QList<Product> getLowStockProducts(int threshold) const;

    // 产品 CRUD 操作
    bool addProduct(Product& product); // 通过非 const 引用传递以更新 ID
    Product getProductById(int id) const;
    QList<Product> getAllProducts() const;
    bool updateProduct(const Product& product);
    bool deleteProduct(int id);
    bool productExists(int id) const; // 辅助函数，检查产品是否存在

    // 类别 CRUD 操作
    bool addCategory(Category& category); // 通过非 const 引用传递以更新 ID
    Category getCategoryById(int id) const;
    QList<Category> getAllCategories() const;
    bool updateCategory(const Category& category);
    bool deleteCategory(int id);
    int getCategoryId(const QString& name) const; // 辅助函数，按名称查找类别 ID

    // 供应商 CRUD 操作
    bool addSupplier(Supplier& supplier); // 通过非 const 引用传递以更新 ID
    Supplier getSupplierById(int id) const;
    QList<Supplier> getAllSuppliers() const;
    bool updateSupplier(const Supplier& supplier);
    bool deleteSupplier(int id);
    int getSupplierId(const QString& name) const; // 辅助函数，按名称查找供应商 ID

private:
    QSqlDatabase m_db;      // 表示连接的 QSqlDatabase 对象
    QString m_dbPath;       // 存储数据库文件的路径
};

#endif // DATABASEMANAGER_H
