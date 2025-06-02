#include "categorydialog.h"
#include "ui_categorydialog.h"

CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog),
    m_editingCategoryId(-1) // 初始化为“添加”模式
{
    ui->setupUi(this);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

void CategoryDialog::setCategory(const Category& category)
{
    m_editingCategoryId = category.id; // 存储类别 ID
    ui->categoryNameLineEdit->setText(category.name);
}

Category CategoryDialog::getCategory() const
{
    Category category;
    category.id = m_editingCategoryId; // 确保 ID 正确
    category.name = ui->categoryNameLineEdit->text().trimmed(); // 获取类别名称
    return category;
}
