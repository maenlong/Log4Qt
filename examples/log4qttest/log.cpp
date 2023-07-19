#include "log.h"

#include "log4qt/basicconfigurator.h"
#include "log4qt/propertyconfigurator.h"

Log * Log::_pInstance = nullptr;
QMutex Log::_mutex;
Log4Qt::Logger * Log::_pLoggerDebug = nullptr;
Log4Qt::Logger * Log::_pLoggerInfo = nullptr;
Log4Qt::Logger * Log::_pLoggerWarn = nullptr;
Log4Qt::Logger * Log::_pLoggerError = nullptr;
QString Log::_configFilePath;

Log::Log(QObject *parent) : QObject(parent)
{
    // 一定要配置文件，不然运行起来会直接当掉
    Log4Qt::BasicConfigurator::configure();
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

void Log::init(QString configFilePath)
{
    _configFilePath = configFilePath;
     Log4Qt::PropertyConfigurator::configure(_configFilePath);

    _pLoggerDebug = Log4Qt::Logger::logger("debug");
    _pLoggerInfo = Log4Qt::Logger::logger("info");
    _pLoggerWarn = Log4Qt::Logger::logger("warn");
    _pLoggerError = Log4Qt::Logger::logger("error");
}

void Log::debug(QString msg)
{
    if(0 == LOG_OUTPUT_MODE)
    {

    }
    else if(1 == LOG_OUTPUT_MODE)
    {
        emit sig_onLogStr(Log4Qt::Level::DEBUG_INT, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qDebug() << msg;
        emit sig_onLogStr(Log4Qt::Level::DEBUG_INT, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
        _pLoggerDebug->debug(msg);
        emit sig_onLogStr(Log4Qt::Level::DEBUG_INT, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qDebug() << msg;
        _pLoggerDebug->debug(msg);
        emit sig_onLogStr(Log4Qt::Level::DEBUG_INT, msg);
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
        emit sig_onLogStr(Log4Qt::Level::INFO_INT, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qInfo() << msg;
        emit sig_onLogStr(Log4Qt::Level::INFO_INT, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
        _pLoggerInfo->info(msg);
        emit sig_onLogStr(Log4Qt::Level::INFO_INT, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qInfo() << msg;
        _pLoggerInfo->info(msg);
        emit sig_onLogStr(Log4Qt::Level::INFO_INT, msg);
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
        emit sig_onLogStr(Log4Qt::Level::WARN_INT, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qWarning() << msg;
        emit sig_onLogStr(Log4Qt::Level::WARN_INT, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
        _pLoggerWarn->warn(msg);
        emit sig_onLogStr(Log4Qt::Level::WARN_INT, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qWarning() << msg;
        _pLoggerWarn->warn(msg);
        emit sig_onLogStr(Log4Qt::Level::WARN_INT, msg);
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
        emit sig_onLogStr(Log4Qt::Level::ERROR_INT, msg);
    }
    else if(2 == LOG_OUTPUT_MODE)
    {
        qCritical() << msg;
        emit sig_onLogStr(Log4Qt::Level::ERROR_INT, msg);
    }
    else if(3 == LOG_OUTPUT_MODE)
    {
        _pLoggerError->error(msg);
        emit sig_onLogStr(Log4Qt::Level::ERROR_INT, msg);
    }
    else if(4 == LOG_OUTPUT_MODE)
    {
        qCritical() << msg;
        _pLoggerError->error(msg);
        emit sig_onLogStr(Log4Qt::Level::ERROR_INT, msg);
    }
    else
    {

    }
}
