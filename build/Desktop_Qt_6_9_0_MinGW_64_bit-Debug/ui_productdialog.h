/********************************************************************************
** Form generated from reading UI file 'productdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTDIALOG_H
#define UI_PRODUCTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProductDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *productNameLineEdit;
    QLabel *label_2;
    QLineEdit *descriptionTextEdit;
    QLabel *label_3;
    QDoubleSpinBox *purchasePriceSpinBox;
    QLabel *label_4;
    QDoubleSpinBox *sellingPriceSpinBox;
    QLabel *label_5;
    QSpinBox *stockQuantitySpinBox;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *supplierComboBox;
    QComboBox *categoryComboBox;

    void setupUi(QDialog *ProductDialog)
    {
        if (ProductDialog->objectName().isEmpty())
            ProductDialog->setObjectName("ProductDialog");
        ProductDialog->resize(686, 495);
        buttonBox = new QDialogButtonBox(ProductDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(300, 340, 81, 71));
        buttonBox->setOrientation(Qt::Orientation::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        buttonBox->setCenterButtons(true);
        formLayoutWidget = new QWidget(ProductDialog);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(100, 100, 481, 223));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        productNameLineEdit = new QLineEdit(formLayoutWidget);
        productNameLineEdit->setObjectName("productNameLineEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, productNameLineEdit);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        descriptionTextEdit = new QLineEdit(formLayoutWidget);
        descriptionTextEdit->setObjectName("descriptionTextEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, descriptionTextEdit);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        purchasePriceSpinBox = new QDoubleSpinBox(formLayoutWidget);
        purchasePriceSpinBox->setObjectName("purchasePriceSpinBox");
        purchasePriceSpinBox->setMaximum(9999.989999999999782);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, purchasePriceSpinBox);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName("label_4");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_4);

        sellingPriceSpinBox = new QDoubleSpinBox(formLayoutWidget);
        sellingPriceSpinBox->setObjectName("sellingPriceSpinBox");
        sellingPriceSpinBox->setMaximum(9999.989999999999782);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, sellingPriceSpinBox);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName("label_5");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_5);

        stockQuantitySpinBox = new QSpinBox(formLayoutWidget);
        stockQuantitySpinBox->setObjectName("stockQuantitySpinBox");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, stockQuantitySpinBox);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName("label_6");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_6);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName("label_7");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, label_7);

        supplierComboBox = new QComboBox(formLayoutWidget);
        supplierComboBox->addItem(QString());
        supplierComboBox->addItem(QString());
        supplierComboBox->addItem(QString());
        supplierComboBox->setObjectName("supplierComboBox");

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, supplierComboBox);

        categoryComboBox = new QComboBox(formLayoutWidget);
        categoryComboBox->addItem(QString());
        categoryComboBox->addItem(QString());
        categoryComboBox->addItem(QString());
        categoryComboBox->setObjectName("categoryComboBox");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, categoryComboBox);


        retranslateUi(ProductDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ProductDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ProductDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ProductDialog);
    } // setupUi

    void retranslateUi(QDialog *ProductDialog)
    {
        ProductDialog->setWindowTitle(QCoreApplication::translate("ProductDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ProductDialog", "\345\220\215\347\247\260\357\274\232", nullptr));
        productNameLineEdit->setPlaceholderText(QCoreApplication::translate("ProductDialog", "Name", nullptr));
        label_2->setText(QCoreApplication::translate("ProductDialog", "\346\217\217\350\277\260\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("ProductDialog", "\351\207\207\350\264\255\344\273\267\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("ProductDialog", "\351\224\200\345\224\256\344\273\267\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("ProductDialog", "\345\272\223\345\255\230\346\225\260\351\207\217\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("ProductDialog", "\347\261\273\345\210\253\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("ProductDialog", "\344\276\233\345\272\224\345\225\206\357\274\232", nullptr));
        supplierComboBox->setItemText(0, QCoreApplication::translate("ProductDialog", "aaa", nullptr));
        supplierComboBox->setItemText(1, QCoreApplication::translate("ProductDialog", "bbb", nullptr));
        supplierComboBox->setItemText(2, QCoreApplication::translate("ProductDialog", "ccc", nullptr));

        categoryComboBox->setItemText(0, QCoreApplication::translate("ProductDialog", "a", nullptr));
        categoryComboBox->setItemText(1, QCoreApplication::translate("ProductDialog", "b", nullptr));
        categoryComboBox->setItemText(2, QCoreApplication::translate("ProductDialog", "c", nullptr));

    } // retranslateUi

};

namespace Ui {
    class ProductDialog: public Ui_ProductDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTDIALOG_H
