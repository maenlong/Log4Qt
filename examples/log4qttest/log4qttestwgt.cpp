#include "log4qttestwgt.h"
#include "ui_log4qttestwgt.h"

Log4QtTestWgt::Log4QtTestWgt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Log4QtTestWgt)
{
    ui->setupUi(this);
}

Log4QtTestWgt::~Log4QtTestWgt()
{
    delete ui;
}

