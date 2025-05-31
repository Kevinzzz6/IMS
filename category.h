#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

struct Category {
    int id;         // 类别的唯一标识符
    QString name;   // 类别名称（例如，“电子产品”、“书籍”）

    Category() : id(-1) {} // 默认构造函数
    Category(int _id, const QString& _name) : id(_id), name(_name) {} // 带参数的
};

#endif // CATEGORY_H
