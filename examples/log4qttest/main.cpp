#include "log4qttestwgt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log4QtTestWgt w;
    w.show();

    return a.exec();
}
