/********************************************************************************
** Form generated from reading UI file 'supplierdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUPPLIERDIALOG_H
#define UI_SUPPLIERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SupplierDialog
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *supplierNameLineEdit;
    QLabel *label_2;
    QTextEdit *contactDetailsTextEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SupplierDialog)
    {
        if (SupplierDialog->objectName().isEmpty())
            SupplierDialog->setObjectName("SupplierDialog");
        SupplierDialog->resize(556, 433);
        gridLayout = new QGridLayout(SupplierDialog);
        gridLayout->setObjectName("gridLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(SupplierDialog);
        label->setObjectName("label");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label);

        supplierNameLineEdit = new QLineEdit(SupplierDialog);
        supplierNameLineEdit->setObjectName("supplierNameLineEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, supplierNameLineEdit);

        label_2 = new QLabel(SupplierDialog);
        label_2->setObjectName("label_2");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_2);

        contactDetailsTextEdit = new QTextEdit(SupplierDialog);
        contactDetailsTextEdit->setObjectName("contactDetailsTextEdit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, contactDetailsTextEdit);

        buttonBox = new QDialogButtonBox(SupplierDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, buttonBox);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(SupplierDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SupplierDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SupplierDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SupplierDialog);
    } // setupUi

    void retranslateUi(QDialog *SupplierDialog)
    {
        SupplierDialog->setWindowTitle(QCoreApplication::translate("SupplierDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SupplierDialog", "\344\276\233\345\272\224\345\225\206\345\220\215\347\247\260\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("SupplierDialog", "\350\201\224\347\263\273\346\226\271\345\274\217\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SupplierDialog: public Ui_SupplierDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUPPLIERDIALOG_H
