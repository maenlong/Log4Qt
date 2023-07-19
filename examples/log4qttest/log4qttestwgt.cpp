#include "log4qttestwgt.h"
#include "ui_log4qttestwgt.h"

#include "log.h"

Log4QtTestWgt::Log4QtTestWgt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Log4QtTestWgt)
{
    ui->setupUi(this);

    connect(Log::instance(), &Log::sig_onLogStr,
            this, &Log4QtTestWgt::slot_onLogStr);
}

Log4QtTestWgt::~Log4QtTestWgt()
{
    delete ui;
}


void Log4QtTestWgt::on_debugBtn_clicked()
{
    LOG_DEBUG(QString("调试测试日志系统当中..."));
    LOG_OBJECT_DEBUG(QString("调试测试日志系统当中..."));
}

void Log4QtTestWgt::on_infoBtn_clicked()
{
    LOG_INFO(QString("信息测试日志系统当中..."));
    LOG_OBJECT_INFO(QString("信息测试日志系统当中..."));
}

void Log4QtTestWgt::on_warnBtn_clicked()
{
    LOG_WARN(QString("警告测试日志系统当中..."));
    LOG_OBJECT_WARN(QString("警告测试日志系统当中..."));
}

void Log4QtTestWgt::on_errorBtn_clicked()
{
    LOG_ERROR(QString("错误测试日志系统当中..."));
    LOG_OBJECT_ERROR(QString("错误测试日志系统当中..."));
}

void Log4QtTestWgt::slot_onLogStr(int level, const QString &msg)
{
    if(level == Log4Qt::Level::DEBUG_INT)
    {
        ui->displayTextEdit->setTextColor(QColor(0, 0, 0, 255));
    }
    else if(level == Log4Qt::Level::INFO_INT)
    {
        ui->displayTextEdit->setTextColor(QColor(0, 155, 0, 255));
    }
    else if(level == Log4Qt::Level::WARN_INT)
    {
        ui->displayTextEdit->setTextColor(QColor(155, 155, 0, 255));
    }
    else if(level == Log4Qt::Level::ERROR_INT)
    {
        ui->displayTextEdit->setTextColor(QColor(255, 0, 0, 255));
    }
    else
    {
        ui->displayTextEdit->setTextColor(QColor(0, 0, 0, 255));
    }

    ui->displayTextEdit->append(/*QString("Level = %1   ").arg(level) + */msg);
}
