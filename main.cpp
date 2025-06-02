// main.cpp
#include "mainwindow.h"
#include "databasemanager.h" // 包含 DatabaseManager 头文件
#include <QApplication>
#include <QMessageBox>      // 用于显示严重错误
#include <QStandardPaths>   // 用于平台无关的目录路径
#include <QDir>// 用于创建目录
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // ✨ 重要修正：使用 argv 而不是 &argv

    // --- 数据库设置 ---
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString dbFilePath = dataPath + "/inventory.db";
    qDebug() << "数据库路径将是：" << dbFilePath;

    DatabaseManager dbManager(dbFilePath);

    if (dbManager.openDatabase()) {
        if (!dbManager.createTables()) {
            QMessageBox::critical(nullptr, "数据库错误",
                                  "无法创建必要的数据库表。应用程序可能无法正常工作。");
            // return 1; // 示例：如果表创建失败则退出
        }
    } else {
        QMessageBox::critical(nullptr, "数据库错误",
                              "无法打开或创建数据库。应用程序现在将退出。");
        return 1; // 如果数据库连接失败则退出
    }
    // --- 数据库设置结束 ---

    MainWindow w;
    w.setDatabaseManager(&dbManager);
    w.show();

    int result = app.exec();
    return result;
}
