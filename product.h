#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString> // Qt 的字符串类，与 UI 元素集成良好

struct Product {
    int id;                 // 唯一标识符，通常来自数据库
    QString name;           // 产品名称
    QString description;    // 详细描述
    double purchasePrice;   // 产品采购价格
    double sellingPrice;    // 产品销售价格
    int categoryId;         // 外键，链接到 Category 表
    int supplierId;         // 外键，链接到 Supplier 表
    int stockQuantity;      // 当前库存数量

    // 默认构造函数：将成员初始化为默认值。
    // 将 id 设置为 -1 可以表示一个新的、未保存的产品。
    Product() : id(-1), purchasePrice(0.0), sellingPrice(0.0),
        categoryId(-1), supplierId(-1), stockQuantity(0) {}

    // 带参数的构造函数（可选，但为方便初始化是好习惯）
    Product(int _id, const QString& _name, const QString& _desc,
            double _purchasePrice, double _sellingPrice,
            int _catId, int _supId, int _stock)
        : id(_id), name(_name), description(_desc),
        purchasePrice(_purchasePrice), sellingPrice(_sellingPrice),
        categoryId(_catId), supplierId(_supId), stockQuantity(_stock) {}
};

#endif // PRODUCT_H
