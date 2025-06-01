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

    // 向数据库添加一个新产品。用新的 ID 更新 product.id。
bool DatabaseManager::addProduct(Product& product) {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法添加产品。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Products (name, description, purchasePrice, sellingPrice, categoryId, supplierId, stockQuantity) "
                  "VALUES (:name, :description, :purchasePrice, :sellingPrice, :categoryId, :supplierId, :stockQuantity)");
    query.bindValue(":name", product.name);
    query.bindValue(":description", product.description);
    query.bindValue(":purchasePrice", product.purchasePrice);
    query.bindValue(":sellingPrice", product.sellingPrice);
    query.bindValue(":categoryId", product.categoryId > 0 ? QVariant(product.categoryId) : QVariant()); // 处理可能为 -1 的无类别情况
    query.bindValue(":supplierId", product.supplierId > 0 ? QVariant(product.supplierId) : QVariant()); // 处理可能为 -1 的无供应商情况
    query.bindValue(":stockQuantity", product.stockQuantity);

    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：addProduct 失败 -" << query.lastError().text();
        return false;
    }
    product.id = query.lastInsertId().toInt(); // 获取新插入产品的 ID [33]
    qDebug() << "产品添加成功，ID 为：" << product.id;
    return true;
}

// 按 ID 检索产品
Product DatabaseManager::getProductById(int id) const {
    Product product; // 默认构造的产品 (id = -1)
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法按 ID 获取产品。";
        return product;
    }
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, description, purchasePrice, sellingPrice, categoryId, supplierId, stockQuantity FROM Products WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        product.id = query.value("id").toInt();
        product.name = query.value("name").toString();
        product.description = query.value("description").toString();
        product.purchasePrice = query.value("purchasePrice").toDouble();
        product.sellingPrice = query.value("sellingPrice").toDouble();
        product.categoryId = query.value("categoryId").isNull() ? -1 : query.value("categoryId").toInt();
        product.supplierId = query.value("supplierId").isNull() ? -1 : query.value("supplierId").toInt();
        product.stockQuantity = query.value("stockQuantity").toInt();
    } else if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：getProductById 失败 -" << query.lastError().text();
    }
    return product;
}

// 从数据库检索所有产品
QList<Product> DatabaseManager::getAllProducts() const {
    QList<Product> products;
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法获取所有产品。";
        return products; // 返回空列表
    }
    QSqlQuery query("SELECT id, name, description, purchasePrice, sellingPrice, categoryId, supplierId, stockQuantity FROM Products", m_db);
    while (query.next()) {
        Product product;
        product.id = query.value("id").toInt();
        product.name = query.value("name").toString();
        product.description = query.value("description").toString();
        product.purchasePrice = query.value("purchasePrice").toDouble();
        product.sellingPrice = query.value("sellingPrice").toDouble();
        product.categoryId = query.value("categoryId").isNull() ? -1 : query.value("categoryId").toInt();
        product.supplierId = query.value("supplierId").isNull() ? -1 : query.value("supplierId").toInt();
        product.stockQuantity = query.value("stockQuantity").toInt();
        products.append(product);
    }
    if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：getAllProducts 失败 -" << query.lastError().text();
    }
    return products;
}

// 更新现有产品的详细信息
bool DatabaseManager::updateProduct(const Product& product) {
    if (!m_db.isOpen() || product.id <= 0) { // 确保数据库已打开且产品具有有效 ID
        qDebug() << "DatabaseManager 错误：数据库未打开或产品 ID 无效，无法更新产品。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("UPDATE Products SET name = :name, description = :description, "
                  "purchasePrice = :purchasePrice, sellingPrice = :sellingPrice, "
                  "categoryId = :categoryId, supplierId = :supplierId, stockQuantity = :stockQuantity "
                  "WHERE id = :id");
    query.bindValue(":name", product.name);
    query.bindValue(":description", product.description);
    query.bindValue(":purchasePrice", product.purchasePrice);
    query.bindValue(":sellingPrice", product.sellingPrice);
    query.bindValue(":categoryId", product.categoryId > 0 ? QVariant(product.categoryId) : QVariant());
    query.bindValue(":supplierId", product.supplierId > 0 ? QVariant(product.supplierId) : QVariant());
    query.bindValue(":stockQuantity", product.stockQuantity);
    query.bindValue(":id", product.id);

    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：updateProduct 失败 -" << query.lastError().text();
        return false;
    }
    qDebug() << "ID 为 " << product.id << " 的产品更新成功。受影响行数：" << query.numRowsAffected();
    return query.numRowsAffected() > 0; // 检查是否实际更新了任何行
}

// 按 ID 删除产品
bool DatabaseManager::deleteProduct(int id) {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法删除产品。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Products WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：deleteProduct 失败 -" << query.lastError().text();
        return false;
    }
    qDebug() << "ID 为 " << id << " 的产品删除成功。受影响行数：" << query.numRowsAffected();
    return query.numRowsAffected() > 0;
}

// 按 ID 检查产品是否存在
bool DatabaseManager::productExists(int id) const {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法检查产品是否存在。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Products WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return true; // 找到产品
    }
    if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：productExists 查询失败 -" << query.lastError().text();
    }
    return false; // 未找到产品或出错
}
      // 添加一个新类别。用新的 ID 更新 category.id。
bool DatabaseManager::addCategory(Category& category) {
    if (!m_db.isOpen()) return false;
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Categories (name) VALUES (:name)");
    query.bindValue(":name", category.name);
    if (!query.exec()) {
        qDebug() << "addCategory 错误：" << query.lastError().text();
        return false;
    }
    category.id = query.lastInsertId().toInt();
    return true;
}

// 按 ID 检索类别
Category DatabaseManager::getCategoryById(int id) const {
    Category category;
    if (!m_db.isOpen()) return category;
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name FROM Categories WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        category.id = query.value("id").toInt();
        category.name = query.value("name").toString();
    } else if (query.lastError().isValid()) {
        qDebug() << "getCategoryById 错误：" << query.lastError().text();
    }
    return category;
}

// 按名称检索类别 ID
int DatabaseManager::getCategoryId(const QString& name) const {
    if (!m_db.isOpen()) return -1;
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Categories WHERE name = :name");
    query.bindValue(":name", name);
    if (query.exec() && query.next()) {
        return query.value("id").toInt();
    } else if (query.lastError().isValid()) {
        qDebug() << "getCategoryId 错误：" << query.lastError().text();
    }
    return -1; // 未找到或出错
}


// 检索所有类别
QList<Category> DatabaseManager::getAllCategories() const {
    QList<Category> categories;
    if (!m_db.isOpen()) return categories;
    QSqlQuery query("SELECT id, name FROM Categories", m_db);
    while (query.next()) {
        Category cat;
        cat.id = query.value("id").toInt();
        cat.name = query.value("name").toString();
        categories.append(cat);
    }
    if (query.lastError().isValid()) {
        qDebug() << "getAllCategories 错误：" << query.lastError().text();
    }
    return categories;
}

// 更新现有类别
bool DatabaseManager::updateCategory(const Category& category) {
    if (!m_db.isOpen() || category.id <= 0) return false;
    QSqlQuery query(m_db);
    query.prepare("UPDATE Categories SET name = :name WHERE id = :id");
    query.bindValue(":name", category.name);
    query.bindValue(":id", category.id);
    if (!query.exec()) {
        qDebug() << "updateCategory 错误：" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

// 按 ID 删除类别
bool DatabaseManager::deleteCategory(int id) {
    if (!m_db.isOpen()) return false;
    // 在删除类别之前，请考虑对使用此别的产品的影响。
    // 当前模式使用 ON DELETE SET NULL，因此 product.categoryId 将变为 NULL。
    // 另一种方法是，如果产品正在使用该类别，则阻止删除，或重新分配产品。
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Categories WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "deleteCategory 错误：" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}
