#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include "category.h"
#include <QDialog>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDialog(QWidget *parent = nullptr);
    ~CategoryDialog();
    Category getCategory() const;
    void setCategory(const Category& category); // 设置类别数据

private:
    Ui::CategoryDialog *ui;
    int m_editingCategoryId; // 用于跟踪正在编辑的类别 ID
};

#endif // CATEGORYDIALOG_H
