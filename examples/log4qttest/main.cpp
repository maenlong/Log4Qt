#include "log4qttestwgt.h"

#include <QApplication>
#include <QThread>
#include <QTextCodec>
#include <QStandardPaths>

#include "log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 必须先设置组织/应用名，QStandardPaths 才能返回包含应用标识的子路径
    QCoreApplication::setOrganizationName("Log4QtOrg");
    QCoreApplication::setApplicationName("Log4QtApp");

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    // 通过 Qt 接口获取平台对应的可写目录，作为日志输出根目录：
    //   Windows: %LOCALAPPDATA%/<Org>/<App>/log
    //   macOS  : ~/Library/Application Support/<Org>/<App>/log
    //   Linux  : ~/.local/share/<Org>/<App>/log
    // 真实客户端可在此处替换为自己的路径策略（环境变量、注册表、命令行参数等）。
    const QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/log";

    Log::instance()->init(QCoreApplication::applicationDirPath() + "/log.conf", logDir);

    QThread::currentThread()->setObjectName("Main_thread");

    Log4QtTestWgt w;
    w.show();

    return a.exec();
}
