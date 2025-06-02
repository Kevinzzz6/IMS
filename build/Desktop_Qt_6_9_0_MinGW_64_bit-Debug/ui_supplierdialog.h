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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SupplierDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *supplierNameLineEdit;
    QTextEdit *contactDetailsTextEdit;

    void setupUi(QDialog *SupplierDialog)
    {
        if (SupplierDialog->objectName().isEmpty())
            SupplierDialog->setObjectName("SupplierDialog");
        SupplierDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(SupplierDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        supplierNameLineEdit = new QLineEdit(SupplierDialog);
        supplierNameLineEdit->setObjectName("supplierNameLineEdit");
        supplierNameLineEdit->setGeometry(QRect(140, 60, 113, 23));
        contactDetailsTextEdit = new QTextEdit(SupplierDialog);
        contactDetailsTextEdit->setObjectName("contactDetailsTextEdit");
        contactDetailsTextEdit->setGeometry(QRect(130, 110, 104, 70));

        retranslateUi(SupplierDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SupplierDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SupplierDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SupplierDialog);
    } // setupUi

    void retranslateUi(QDialog *SupplierDialog)
    {
        SupplierDialog->setWindowTitle(QCoreApplication::translate("SupplierDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SupplierDialog: public Ui_SupplierDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUPPLIERDIALOG_H
