#include "log4qttestwgt.h"

#include <QApplication>

#include "logger.h"
#include "basicconfigurator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log4Qt::BasicConfigurator::configure();
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    log->debug("debug!");
    log->info("information!");
    log->warn("warn");

    Log4QtTestWgt w;
    w.show();

    return a.exec();
}
