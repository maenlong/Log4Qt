#include "log4qttestwgt.h"

#include <QApplication>
#include <QThread>
#include <QTextCodec>

#include "log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Log::instance()->init(QCoreApplication::applicationDirPath() + "/" +"log.conf");

    QThread::currentThread()->setObjectName("Main_thread");

    Log4QtTestWgt w;
    w.show();

    return a.exec();
}
