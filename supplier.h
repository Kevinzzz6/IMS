#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <QString>

struct Supplier {
    int id;                     // 供应商的唯一标识符
    QString name;               // 供应商公司名称
    QString contactDetails;     // 联系信息（例如，电子邮件、电话、地址）

    Supplier() : id(-1) {} // 默认构造函数
    Supplier(int _id, const QString& _name, const QString& _contact)
        : id(_id), name(_name), contactDetails(_contact) {} // 带参数的
};

#endif // SUPPLIER_H
