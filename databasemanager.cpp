#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString& path) : m_dbPath(path) {
    // 获取默认数据库连接，或添加一个命名连接
    // 必须在 setDatabaseName() 之前调用 addDatabase()
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE"); // 指定 SQLite 驱动程序 [4]
    }
    m_db.setDatabaseName(m_dbPath); // 设置数据库文件的路径
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close(); // 确保在销毁时关闭数据库
    }
}

bool DatabaseManager::openDatabase() {
    if (!m_db.open()) {
        // 记录详细的错误信息
        qDebug() << "DatabaseManager 错误：数据库连接失败 -"
                 << m_db.lastError().driverText() << m_db.lastError().databaseText();
        return false;
    }
    qDebug() << "数据库：成功连接到" << m_dbPath;
    return true;
}

void DatabaseManager::closeDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "数据库：连接已关闭。";
    }
}

QSqlDatabase DatabaseManager::getDatabase() const {
    return m_db;
}

bool DatabaseManager::createTables() {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开。无法创建表。";
        return false;
    }

    QSqlQuery query(m_db); // 将查询与我们的数据库连接关联起来

    // 产品表
    // SQLite 使用 INTEGER PRIMARY KEY AUTOINCREMENT 实现自增 ID。
    // TEXT 对应 QString，REAL 对应 double，INTEGER 对应 int。
    // FOREIGN KEY 约束定义表之间的关系。
    QString createProductsTableSQL = R"(
    CREATE TABLE IF NOT EXISTS Products (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        description TEXT,
        purchasePrice REAL DEFAULT 0.0,
        sellingPrice REAL DEFAULT 0.0,
        categoryId INTEGER,
        supplierId INTEGER,
        stockQuantity INTEGER DEFAULT 0,
        FOREIGN KEY (categoryId) REFERENCES Categories (id) ON DELETE SET NULL ON UPDATE CASCADE,
        FOREIGN KEY (supplierId) REFERENCES Suppliers (id) ON DELETE SET NULL ON UPDATE CASCADE
    );)"; // [2] 模式思路

    if (!query.exec(createProductsTableSQL)) {
        qDebug() << "DatabaseManager 错误：无法创建 'Products' 表 -" << query.lastError().text();
        return false;
    } else {
        qDebug() << "'Products' 表创建成功或已存在。";
    }

    // 类别表
    QString createCategoriesTableSQL = R"(
    CREATE TABLE IF NOT EXISTS Categories (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE
    );)"; // UNIQUE 约束确保类别名称不重复

    if (!query.exec(createCategoriesTableSQL)) {
        qDebug() << "DatabaseManager 错误：无法创建 'Categories' 表 -" << query.lastError().text();
        return false;
    } else {
        qDebug() << "'Categories' 表创建成功或已存在。";
    }

    // 供应商表
    QString createSuppliersTableSQL = R"(
    CREATE TABLE IF NOT EXISTS Suppliers (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL UNIQUE,
        contactDetails TEXT
    );)"; // 供应商名称的 UNIQUE 约束

    if (!query.exec(createSuppliersTableSQL)) {
        qDebug() << "DatabaseManager 错误：无法创建 'Suppliers' 表 -" << query.lastError().text();
        return false;
    } else {
        qDebug() << "'Suppliers' 表创建成功或已存在。";
    }

    // 如果默认类别不存在，则添加示例
    query.prepare("INSERT OR IGNORE INTO Categories (name) VALUES (:name)");
    query.bindValue(":name", "未分类");
    if (!query.exec()) {
        qDebug() << "DatabaseManager 警告：无法插入默认 '未分类' 类别 -" << query.lastError().text();
    }

    // 如果默认供应商不存在，则添加示例
    query.prepare("INSERT OR IGNORE INTO Suppliers (name, contactDetails) VALUES (:name, :contact)");
    query.bindValue(":name", "默认供应商");
    query.bindValue(":contact", "N/A");
    if (!query.exec()) {
        qDebug() << "DatabaseManager 警告：无法插入默认 '默认供应商' -" << query.lastError().text();
    }


    qDebug() << "所有表已检查/创建成功。";
    return true;
}
