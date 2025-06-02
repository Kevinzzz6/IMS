// main.cpp
#include "mainwindow.h"
#include "databasemanager.h" // 包含 DatabaseManager 头文件
#include <QApplication>
#include <QMessageBox>      // 用于显示严重错误
#include <QStandardPaths>   // 用于平台无关的目录路径
#include <QDir>             // 用于创建目录

int main(int argc, char *argv) {
    QApplication app(argc, &argv);

    // --- 数据库设置 ---
    // 在应用程序的可写数据位置构建路径
    // 这对于已部署的应用程序是一个好习惯。
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) { // 如果目录不存在则创建
        dir.mkpath(".");
    }
    QString dbFilePath = dataPath + "/inventory.db"; // 定义数据库文件名
    qDebug() << "数据库路径将是：" << dbFilePath;

    DatabaseManager dbManager(dbFilePath); // 创建 DatabaseManager 实例

    if (dbManager.openDatabase()) { // 打开数据库
        if (!dbManager.createTables()) { // 如果表不存在则创建
            QMessageBox::critical(nullptr, "数据库错误",
                                  "无法创建必要的数据库表。应用程序可能无法正常工作。");
            // 决定应用程序是应该退出还是以有限功能继续运行
            // return 1; // 示例：如果表创建失败则退出
        }
        // 数据库现已打开，表已准备就绪。
        // 可以在应用程序的整个生命周期内保持打开状态，或者根据需要关闭。
        // 对于此应用程序，保持打开状态即可。
    } else {
        QMessageBox::critical(nullptr, "数据库错误",
                              "无法打开或创建数据库。应用程序现在将退出。");
        return 1; // 如果数据库连接失败则退出
    }
    // --- 数据库设置结束 ---

    MainWindow w;
    w.setDatabaseManager(&dbManager); // 将 dbManager 实例传递给 MainWindow
    w.show();

    int result = app.exec();
    // dbManager.closeDatabase(); // DatabaseManager 析构函数将处理关闭
    return result;
}
