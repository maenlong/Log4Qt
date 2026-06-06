#include "log.h"

#ifdef HAVE_LOG4QT
#include "log4qt/basicconfigurator.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/helpers/properties.h"
#endif

#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QDebug>

// 日志等级常量：无 log4qt 时仍要给 sig_onLogStr 提供有效 int（与 Log4Qt::Level 对齐）
namespace {
#ifdef HAVE_LOG4QT
    const int kLvlDebug = Log4Qt::Level::DEBUG_INT;
    const int kLvlInfo  = Log4Qt::Level::INFO_INT;
    const int kLvlWarn  = Log4Qt::Level::WARN_INT;
    const int kLvlError = Log4Qt::Level::ERROR_INT;
#else
    const int kLvlDebug = 96;
    const int kLvlInfo  = 128;
    const int kLvlWarn  = 150;
    const int kLvlError = 182;
#endif
}

Log * Log::_pInstance = nullptr;
QMutex Log::_mutex;
#ifdef HAVE_LOG4QT
Log4Qt::Logger * Log::_pLoggerDebug = nullptr;
Log4Qt::Logger * Log::_pLoggerInfo = nullptr;
Log4Qt::Logger * Log::_pLoggerWarn = nullptr;
Log4Qt::Logger * Log::_pLoggerError = nullptr;
#endif
QString Log::_configFilePath;

Log::Log(QObject *parent) : QObject(parent)
{
#ifdef HAVE_LOG4QT
    // 一定要配置文件，不然运行起来会直接当掉
    Log4Qt::BasicConfigurator::configure();
#endif
}

Log *Log::instance()
{
    if(!_pInstance)
    {
        QMutexLocker mutexLocker(&_mutex);
        if(!_pInstance)
        {
            Log *pInstance = new Log();
            _pInstance = pInstance;
        }
    }
    return _pInstance;
}

void Log::init(QString configFilePath, QString logDir)
{
    _configFilePath = configFilePath;

#ifndef HAVE_LOG4QT
    Q_UNUSED(logDir);
    qWarning() << "[Log] HAVE_LOG4QT not defined; init is a no-op."
                  " Log subsystem will not write any file.";
    return;
#else
    // 1) 校验：logDir 必传，由调用方显式决定路径策略
    if (logDir.isEmpty())
    {
        qWarning() << "[Log] init aborted: logDir must not be empty."
                      " Caller must decide the log directory explicitly"
                      " (e.g. via QStandardPaths::writableLocation).";
        return;
    }

    // 2) 规范化路径并确保目录存在
    //    - 统一为正斜杠：Java properties 格式下 '\' 会被解释为转义
    //    - RollingFileAppender 不会自动建目录，必须先确保存在，否则文件打不开
    QString effectiveLogDir = logDir;
    effectiveLogDir.replace(QLatin1Char('\\'), QLatin1Char('/'));
    if (!QDir().mkpath(effectiveLogDir))
    {
        qWarning() << "[Log] failed to create log dir:" << effectiveLogDir;
    }

    // 3) 读取 log.conf 文本
    QFile f(configFilePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "[Log] open log.conf failed:" << configFilePath
                   << "error:" << f.errorString();
        return;
    }
    QString text = QString::fromUtf8(f.readAll());
    f.close();

    // 4) 替换占位符
    text.replace(QStringLiteral("${LOG_DIR}"), effectiveLogDir);

    // 5) 用 in-memory Properties 喂给 PropertyConfigurator
    //    （避免落盘临时文件，且不依赖 Log4Qt 端的环境变量替换机制）
    QByteArray bytes = text.toUtf8();
    QBuffer buf(&bytes);
    buf.open(QIODevice::ReadOnly);
    Log4Qt::Properties props;
    props.load(&buf);
    Log4Qt::PropertyConfigurator::configure(props);

    _pLoggerDebug = Log4Qt::Logger::logger("debug");
    _pLoggerInfo = Log4Qt::Logger::logger("info");
    _pLoggerWarn = Log4Qt::Logger::logger("warn");
    _pLoggerError = Log4Qt::Logger::logger("error");
#endif
}

void Log::debug(QString msg)
{
    if(0 == LOG_OUTPUT_MODE)
    {

    }
    else if(1 == LOG_OUTPUT_MODE)
    {
        emit sig_onLogStr(kLvlDebug, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qDebug() << msg;
        emit sig_onLogStr(kLvlDebug, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
#ifdef HAVE_LOG4QT
        if (_pLoggerDebug)
            _pLoggerDebug->debug(msg);
#endif
        emit sig_onLogStr(kLvlDebug, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qDebug() << msg;
#ifdef HAVE_LOG4QT
        if (_pLoggerDebug)
            _pLoggerDebug->debug(msg);
#endif
        emit sig_onLogStr(kLvlDebug, msg);
    }
    else
    {

    }
}

void Log::info(QString msg)
{
    if(0 == LOG_OUTPUT_MODE)
    {

    }
    else if(1 == LOG_OUTPUT_MODE)
    {
        emit sig_onLogStr(kLvlInfo, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qInfo() << msg;
        emit sig_onLogStr(kLvlInfo, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
#ifdef HAVE_LOG4QT
        if (_pLoggerInfo)
            _pLoggerInfo->info(msg);
#endif
        emit sig_onLogStr(kLvlInfo, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qInfo() << msg;
#ifdef HAVE_LOG4QT
        if (_pLoggerInfo)
            _pLoggerInfo->info(msg);
#endif
        emit sig_onLogStr(kLvlInfo, msg);
    }
    else
    {

    }
}

void Log::warn(QString msg)
{

    if(0 == LOG_OUTPUT_MODE)
    {

    }
    else if(1 == LOG_OUTPUT_MODE)
    {
        emit sig_onLogStr(kLvlWarn, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qWarning() << msg;
        emit sig_onLogStr(kLvlWarn, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
#ifdef HAVE_LOG4QT
        if (_pLoggerWarn)
            _pLoggerWarn->warn(msg);
#endif
        emit sig_onLogStr(kLvlWarn, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qWarning() << msg;
#ifdef HAVE_LOG4QT
        if (_pLoggerWarn)
            _pLoggerWarn->warn(msg);
#endif
        emit sig_onLogStr(kLvlWarn, msg);
    }
    else
    {

    }
}

void Log::error(QString msg)
{
    if(0 == LOG_OUTPUT_MODE)
    {

    }
    else if(1 == LOG_OUTPUT_MODE)
    {
        emit sig_onLogStr(kLvlError, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qCritical() << msg;
        emit sig_onLogStr(kLvlError, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
#ifdef HAVE_LOG4QT
        if (_pLoggerError)
            _pLoggerError->error(msg);
#endif
        emit sig_onLogStr(kLvlError, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qCritical() << msg;
#ifdef HAVE_LOG4QT
        if (_pLoggerError)
            _pLoggerError->error(msg);
#endif
        emit sig_onLogStr(kLvlError, msg);
    }
    else
    {

    }
}
