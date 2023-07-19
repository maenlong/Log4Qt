#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QThread>

#include "log4qt/logger.h"

#define LOG_OUTPUT_MODE 2       //0.不处理 1.只回调 2.只打印 3.写文件 4.全处理


// 以文件行列记录信息，非以类名对象形式
#define LOG_DEBUG(msg) { \
                                            QString retStr; \
                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                               .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                               .arg(QThread::currentThread()->objectName())  \
                                               .arg("DEBUG")  \
                                               .arg(msg); \
                                            Log::instance()->debug(retStr); \
                                            }

#define LOG_INFO(msg)  { \
                                            QString retStr; \
                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                               .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                               .arg(QThread::currentThread()->objectName())  \
                                               .arg("INFO")  \
                                               .arg(msg); \
                                            Log::instance()->info(retStr); \
                                            }

#define LOG_WARN(msg) { \
                                            QString retStr; \
                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                               .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                               .arg(QThread::currentThread()->objectName())  \
                                               .arg("WARN")  \
                                               .arg(msg); \
                                            Log::instance()->warn(retStr); \
                                            }

#define LOG_ERROR(msg) { \
                                            QString retStr; \
                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                               .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                               .arg(QThread::currentThread()->objectName())  \
                                               .arg("ERROR")  \
                                               .arg(msg); \
                                            Log::instance()->error(retStr); \
                                            }


// 以类名对象形式记录信息
#define LOG_OBJECT_DEBUG(msg) { \
                                                            QString retStr; \
                                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                                               .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                                               .arg(QThread::currentThread()->objectName())  \
                                                               .arg("DEBUG")  \
                                                               .arg(msg); \
                                                            Log::instance()->debug(retStr); \
                                                            }

#define LOG_OBJECT_INFO(msg)  { \
                                                        QString retStr; \
                                                        retStr = QString("[%1][%2][%3][%4]%5")  \
                                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                                           .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                                           .arg(QThread::currentThread()->objectName())  \
                                                           .arg("INFO")  \
                                                           .arg(msg); \
                                                        Log::instance()->info(retStr); \
                                                        }

#define LOG_OBJECT_WARN(msg)  { \
                                                            QString retStr; \
                                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                                               .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                                               .arg(QThread::currentThread()->objectName())  \
                                                               .arg("WARN")  \
                                                               .arg(msg); \
                                                            Log::instance()->warn(retStr); \
                                                            }

#define LOG_OBJECT_ERROR(msg)  { \
                                                            QString retStr; \
                                                            retStr = QString("[%1][%2][%3][%4]%5")  \
                                                               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                                               .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                                               .arg(QThread::currentThread()->objectName())  \
                                                               .arg("ERROR")  \
                                                               .arg(msg); \
                                                            Log::instance()->error(retStr); \
                                                            }


/************************************************************\
 * 类名：Log
 * 描述：封装log4qt开源库类
 * 注意：log分为4级别：DEBUG > INFO > WARN > ERROR
 * 信号：
 * 函数：
 *      instance() - 获取日志唯一实例
 * 槽函数：
 *      slot_init() - 初始化加载配置文件
 *      slot_debug() - 调试级别日志
 *      slot_info() - 信息级别日志
 *      slot_warn() - 警告级别日志
 *      slot_error() - 错误级别日志
 *
 * 作者：maenlong
\************************************************************/


class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);

signals:
    void sig_onLogStr(int level, const QString &msg);

public slots:
    static Log * instance();

public slots:
    void init(QString configFilePath);

public slots:
    void debug(QString msg);
    void info(QString msg);
    void warn(QString msg);
    void error(QString msg);

private:
    static Log *_pInstance;
    static QMutex _mutex;
    static Log4Qt::Logger * _pLoggerDebug;
    static Log4Qt::Logger * _pLoggerInfo;
    static Log4Qt::Logger * _pLoggerWarn;
    static Log4Qt::Logger * _pLoggerError;
    static QString _configFilePath;

};
#endif // LOG_H
