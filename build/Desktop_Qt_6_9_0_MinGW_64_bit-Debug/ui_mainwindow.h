/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *mainTabWidget;
    QWidget *productsTab;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_2;
    QComboBox *productCategoryFilterComboBox;
    QLineEdit *productSearchLineEdit;
    QLabel *label;
    QLabel *label_3;
    QPushButton *clearProductFilterButton;
    QTableView *productsTableView;
    QPushButton *addProductButton;
    QPushButton *editProductButton;
    QPushButton *deleteProductButton;
    QWidget *categoriesTab;
    QGridLayout *gridLayout_3;
    QPushButton *editCategoryButton;
    QTableView *categoriesTableView;
    QPushButton *deleteCategoryButton;
    QPushButton *addCategoryButton;
    QWidget *suppliersTab;
    QGridLayout *gridLayout_4;
    QTableView *suppliersTableView;
    QPushButton *addSupplierButton;
    QPushButton *editSupplierButton;
    QPushButton *deleteSupplierButton;
    QWidget *reportsTab;
    QGridLayout *gridLayout_5;
    QLabel *label_2;
    QPushButton *generateLowStockReportButton;
    QTableView *lowStockTableView;
    QSpinBox *lowStockThresholdSpinBox;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 724);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        mainTabWidget = new QTabWidget(centralwidget);
        mainTabWidget->setObjectName("mainTabWidget");
        productsTab = new QWidget();
        productsTab->setObjectName("productsTab");
        gridLayout_6 = new QGridLayout(productsTab);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        productCategoryFilterComboBox = new QComboBox(productsTab);
        productCategoryFilterComboBox->setObjectName("productCategoryFilterComboBox");

        gridLayout_2->addWidget(productCategoryFilterComboBox, 6, 1, 1, 1);

        productSearchLineEdit = new QLineEdit(productsTab);
        productSearchLineEdit->setObjectName("productSearchLineEdit");

        gridLayout_2->addWidget(productSearchLineEdit, 1, 1, 1, 1);

        label = new QLabel(productsTab);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(productsTab);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 6, 0, 1, 1);

        clearProductFilterButton = new QPushButton(productsTab);
        clearProductFilterButton->setObjectName("clearProductFilterButton");

        gridLayout_2->addWidget(clearProductFilterButton, 7, 0, 1, 2);


        gridLayout_6->addLayout(gridLayout_2, 0, 0, 1, 1);

        productsTableView = new QTableView(productsTab);
        productsTableView->setObjectName("productsTableView");

        gridLayout_6->addWidget(productsTableView, 1, 0, 1, 1);

        addProductButton = new QPushButton(productsTab);
        addProductButton->setObjectName("addProductButton");

        gridLayout_6->addWidget(addProductButton, 2, 0, 1, 1);

        editProductButton = new QPushButton(productsTab);
        editProductButton->setObjectName("editProductButton");

        gridLayout_6->addWidget(editProductButton, 3, 0, 1, 1);

        deleteProductButton = new QPushButton(productsTab);
        deleteProductButton->setObjectName("deleteProductButton");

        gridLayout_6->addWidget(deleteProductButton, 4, 0, 1, 1);

        mainTabWidget->addTab(productsTab, QString());
        categoriesTab = new QWidget();
        categoriesTab->setObjectName("categoriesTab");
        gridLayout_3 = new QGridLayout(categoriesTab);
        gridLayout_3->setObjectName("gridLayout_3");
        editCategoryButton = new QPushButton(categoriesTab);
        editCategoryButton->setObjectName("editCategoryButton");

        gridLayout_3->addWidget(editCategoryButton, 3, 0, 1, 1);

        categoriesTableView = new QTableView(categoriesTab);
        categoriesTableView->setObjectName("categoriesTableView");

        gridLayout_3->addWidget(categoriesTableView, 1, 0, 1, 1);

        deleteCategoryButton = new QPushButton(categoriesTab);
        deleteCategoryButton->setObjectName("deleteCategoryButton");

        gridLayout_3->addWidget(deleteCategoryButton, 4, 0, 1, 1);

        addCategoryButton = new QPushButton(categoriesTab);
        addCategoryButton->setObjectName("addCategoryButton");

        gridLayout_3->addWidget(addCategoryButton, 2, 0, 1, 1);

        mainTabWidget->addTab(categoriesTab, QString());
        suppliersTab = new QWidget();
        suppliersTab->setObjectName("suppliersTab");
        gridLayout_4 = new QGridLayout(suppliersTab);
        gridLayout_4->setObjectName("gridLayout_4");
        suppliersTableView = new QTableView(suppliersTab);
        suppliersTableView->setObjectName("suppliersTableView");

        gridLayout_4->addWidget(suppliersTableView, 0, 0, 1, 1);

        addSupplierButton = new QPushButton(suppliersTab);
        addSupplierButton->setObjectName("addSupplierButton");

        gridLayout_4->addWidget(addSupplierButton, 1, 0, 1, 1);

        editSupplierButton = new QPushButton(suppliersTab);
        editSupplierButton->setObjectName("editSupplierButton");

        gridLayout_4->addWidget(editSupplierButton, 2, 0, 1, 1);

        deleteSupplierButton = new QPushButton(suppliersTab);
        deleteSupplierButton->setObjectName("deleteSupplierButton");

        gridLayout_4->addWidget(deleteSupplierButton, 3, 0, 1, 1);

        mainTabWidget->addTab(suppliersTab, QString());
        reportsTab = new QWidget();
        reportsTab->setObjectName("reportsTab");
        gridLayout_5 = new QGridLayout(reportsTab);
        gridLayout_5->setObjectName("gridLayout_5");
        label_2 = new QLabel(reportsTab);
        label_2->setObjectName("label_2");

        gridLayout_5->addWidget(label_2, 1, 1, 1, 1);

        generateLowStockReportButton = new QPushButton(reportsTab);
        generateLowStockReportButton->setObjectName("generateLowStockReportButton");

        gridLayout_5->addWidget(generateLowStockReportButton, 4, 1, 1, 1);

        lowStockTableView = new QTableView(reportsTab);
        lowStockTableView->setObjectName("lowStockTableView");

        gridLayout_5->addWidget(lowStockTableView, 3, 1, 1, 1);

        lowStockThresholdSpinBox = new QSpinBox(reportsTab);
        lowStockThresholdSpinBox->setObjectName("lowStockThresholdSpinBox");
        lowStockThresholdSpinBox->setValue(10);

        gridLayout_5->addWidget(lowStockThresholdSpinBox, 2, 1, 1, 1);

        mainTabWidget->addTab(reportsTab, QString());

        gridLayout->addWidget(mainTabWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        menubar->setDefaultUp(true);
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        mainTabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
        productSearchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\346\214\211\344\272\247\345\223\201\345\220\215\347\247\260\346\250\241\347\263\212\346\220\234\347\264\242...", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242\344\272\247\345\223\201\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\214\211\347\261\273\345\210\253\347\255\233\351\200\211\357\274\232", nullptr));
        clearProductFilterButton->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\347\255\233\351\200\211", nullptr));
        addProductButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\344\272\247\345\223\201", nullptr));
        editProductButton->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\346\211\200\351\200\211\344\272\247\345\223\201", nullptr));
        deleteProductButton->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\346\211\200\351\200\211\344\272\247\345\223\201", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(productsTab), QCoreApplication::translate("MainWindow", "\344\272\247\345\223\201", nullptr));
        editCategoryButton->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\346\211\200\351\200\211\347\261\273\345\210\253", nullptr));
        deleteCategoryButton->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\346\211\200\351\200\211\347\261\273\345\210\253", nullptr));
        addCategoryButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\261\273\345\210\253", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(categoriesTab), QCoreApplication::translate("MainWindow", "\347\261\273\345\210\253", nullptr));
        addSupplierButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\344\276\233\345\272\224\345\225\206", nullptr));
        editSupplierButton->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\346\211\200\351\200\211\344\276\233\345\272\224\345\225\206", nullptr));
        deleteSupplierButton->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\346\211\200\351\200\211\344\276\233\345\272\224\345\225\206", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(suppliersTab), QCoreApplication::translate("MainWindow", "\344\276\233\345\272\224\345\225\206", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\275\216\345\272\223\345\255\230\351\230\210\345\200\274\357\274\232", nullptr));
        generateLowStockReportButton->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\344\275\216\345\272\223\345\255\230\346\212\245\350\241\250", nullptr));
        mainTabWidget->setTabText(mainTabWidget->indexOf(reportsTab), QCoreApplication::translate("MainWindow", "\346\212\245\350\241\250", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
