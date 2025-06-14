QT += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = IMS # 或者您的可执行文件名
TEMPLATE = app
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    categorydialog.cpp \
    databasemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    productdialog.cpp \
    supplierdialog.cpp

HEADERS += \
    category.h \
    categorydialog.h \
    databasemanager.h \
    mainwindow.h \
    product.h \
    productdialog.h \
    supplier.h \
    supplierdialog.h

FORMS += \
    categorydialog.ui \
    mainwindow.ui \
    productdialog.ui \
    supplierdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
