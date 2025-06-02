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

QList<Product> DatabaseManager::getLowStockProducts(int threshold) const {
    QList<Product> products;
    if (!m_db.isOpen()) {
        qDebug() << "数据库未打开，无法获取低库存产品";
        return products;
    }
    QSqlQuery query(m_db);
    // SQL 查询，查找 stockQuantity 小于或等于阈值的产品 [2, 73]
    query.prepare("SELECT id, name, description, purchasePrice, sellingPrice, categoryId, supplierId, stockQuantity "
                  "FROM Products WHERE stockQuantity <= :threshold ORDER BY stockQuantity ASC");
    query.bindValue(":threshold", threshold);

    if (query.exec()) {
        while (query.next()) {
            Product product;
            product.id = query.value("id").toInt();
            product.name = query.value("name").toString();
            // ... (像 getAllProducts 方法中那样，确保从查询结果中填充产品对象的其他所有相关字段，
            // 例如 description, purchasePrice, sellingPrice 等，以确保报表所需的产品信息是完整的) ...

            // 精确获取库存数量，这是生成低库存报表的关键数据。
            product.stockQuantity = query.value("stockQuantity").toInt();

            // 处理可能为空的 categoryId 外键。
            // 如果数据库中的 categoryId 为 NULL (表示产品未分配类别),
            // 则在 Product 对象中将其设置为 -1。这是一种常用的约定，
            // 用于在应用程序层面表示“无类别”或“未指定”，
            // 避免了直接使用可能引起混淆的0，并允许后续逻辑明确处理这种情况。
            product.categoryId = query.value("categoryId").isNull() ? -1 : query.value("categoryId").toInt();

            // 同样地处理可能为空的 supplierId 外键。
            // 如果 supplierId 在数据库中为 NULL (表示产品无指定供应商),
            // 则在 Product 对象中将其设置为 -1。这种处理方式确保了
            // 即使外键数据缺失，产品对象也能被正确构建和使用，
            // 例如在报表中显示“无供应商”而不是因空值导致错误。
            product.supplierId = query.value("supplierId").isNull() ? -1 : query.value("supplierId").toInt();
            products.append(product);
        }
        qDebug() << "低库存产品查询成功，数量：" << products.size();
    } else {
        qDebug() << "获取低库存产品时出错：" << query.lastError().text();
    }
    return products;
}

// 添加一个新供应商。用新的 ID 更新 supplier.id。
bool DatabaseManager::addSupplier(Supplier& supplier) {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法添加供应商。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Suppliers (name, contactDetails) VALUES (:name, :contactDetails)");
    query.bindValue(":name", supplier.name);
    query.bindValue(":contactDetails", supplier.contactDetails);

    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：addSupplier 失败 -" << query.lastError().text();
        return false;
    }
    supplier.id = query.lastInsertId().toInt();
    qDebug() << "供应商添加成功，ID 为：" << supplier.id;
    return true;
}

Supplier DatabaseManager::getSupplierById(int id) const {
    Supplier supplier; // 默认构造 (id = -1)
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法按 ID 获取供应商。";
        return supplier;
    }
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, contactDetails FROM Suppliers WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        supplier.id = query.value("id").toInt();
        supplier.name = query.value("name").toString();
        supplier.contactDetails = query.value("contactDetails").toString();
    } else if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：getSupplierById 失败 -" << query.lastError().text();
    }
    return supplier;
}

QList<Supplier> DatabaseManager::getAllSuppliers() const { // ✨ 实现这个关键函数
    QList<Supplier> suppliers;
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法获取所有供应商。";
        return suppliers;
    }
    QSqlQuery query("SELECT id, name, contactDetails FROM Suppliers ORDER BY name ASC", m_db); // 按名称排序
    while (query.next()) {
        Supplier sup;
        sup.id = query.value("id").toInt();
        sup.name = query.value("name").toString();
        sup.contactDetails = query.value("contactDetails").toString();
        suppliers.append(sup);
    }
    if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：getAllSuppliers 失败 -" << query.lastError().text();
    }
    return suppliers;
}

bool DatabaseManager::updateSupplier(const Supplier& supplier) {
    if (!m_db.isOpen() || supplier.id <= 0) {
        qDebug() << "DatabaseManager 错误：数据库未打开或供应商 ID 无效，无法更新供应商。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("UPDATE Suppliers SET name = :name, contactDetails = :contactDetails WHERE id = :id");
    query.bindValue(":name", supplier.name);
    query.bindValue(":contactDetails", supplier.contactDetails);
    query.bindValue(":id", supplier.id);

    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：updateSupplier 失败 -" << query.lastError().text();
        return false;
    }
    qDebug() << "ID 为 " << supplier.id << " 的供应商更新成功。受影响行数：" << query.numRowsAffected();
    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteSupplier(int id) {
    if (!m_db.isOpen()) {
        qDebug() << "DatabaseManager 错误：数据库未打开，无法删除供应商。";
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Suppliers WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "DatabaseManager 错误：deleteSupplier 失败 -" << query.lastError().text();
        return false;
    }
    qDebug() << "ID 为 " << id << " 的供应商删除成功。受影响行数：" << query.numRowsAffected();
    return query.numRowsAffected() > 0;
}

// getSupplierId (如果之前没有实现，也需要实现)
int DatabaseManager::getSupplierId(const QString& name) const {
    if (!m_db.isOpen()) return -1;
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Suppliers WHERE name = :name");
    query.bindValue(":name", name);
    if (query.exec() && query.next()) {
        return query.value("id").toInt();
    } else if (query.lastError().isValid()) {
        qDebug() << "DatabaseManager 错误：getSupplierId 失败 -" << query.lastError().text();
    }
    return -1; // 未找到或出错
}
