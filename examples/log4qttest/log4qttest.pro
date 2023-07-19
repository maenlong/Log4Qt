QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#-----------------------------------------------------------------------

# 定义 Log4Qt 源码根目录
LOG4QT_PATH = ../../
# 定义所需的宏
DEFINES += LOG4QT_LIBRARY
# 指定编译项目时应该被搜索的 #include 目录
INCLUDEPATH += $$LOG4QT_PATH/src \
               $$LOG4QT_PATH/src/log4qt \
               $$LOG4QT_PATH/include \
               $$LOG4QT_PATH/include/log4qt
# 将 Log4Qt 源代码添加至项目中
include($$LOG4QT_PATH/src/log4qt/log4qt.pri)
include($$LOG4QT_PATH/build.pri)
include($$LOG4QT_PATH/g++.pri)

#-----------------------------------------------------------------------

SOURCES += \
    main.cpp \
    log.cpp \
    log4qttestwgt.cpp

HEADERS += \
    log.h \
    log4qttestwgt.h

FORMS += \
    log4qttestwgt.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
