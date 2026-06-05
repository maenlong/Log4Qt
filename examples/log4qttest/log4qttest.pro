QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 让Release版本生成调试信息
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

#-----------------------------------------------------------------------

# 定义 Log4Qt 源码根目录
#LOG4QT_PATH = ../../
# 链接 Log4Qt 静态库时，消费方必须定义 LOG4QT_STATIC，
# 否则头文件中的导出宏会按动态库(dllimport/dllexport)展开，导致链接异常
DEFINES += LOG4QT_STATIC

# 指定编译项目时应该被搜索的 #include 目录
#INCLUDEPATH += $$LOG4QT_PATH/src \
#               $$LOG4QT_PATH/src/log4qt \
#               $$LOG4QT_PATH/include \
#               $$LOG4QT_PATH/include/log4qt

# 将 Log4Qt 源代码添加至项目中
#include($$LOG4QT_PATH/build.pri)
#include($$LOG4QT_PATH/g++.pri)
#include($$LOG4QT_PATH/src/log4qt/log4qt.pri)

# 根据 平台 + 架构 选择对应的预编译静态库
# 目录约定（与 qBreakpad 项目保持一致）：
#   windows : x86 / x64        （内部再分 debug / release）
#   mac     : x86_64 / arm64
#   linux   : x86_64 / arm64
INCLUDEPATH += $$PWD/log4qtlib/include
DEPENDPATH  += $$PWD/log4qtlib/include

# ---- Windows ----
win32 {
    contains(QT_ARCH, x86_64)|contains(QMAKE_TARGET.arch, x86_64) {
        LOG4QT_WIN_ARCH = x64
    } else {
        LOG4QT_WIN_ARCH = x86
    }
    CONFIG(release, debug|release) {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/windows/$$LOG4QT_WIN_ARCH/release
    } else {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/windows/$$LOG4QT_WIN_ARCH/debug
    }
    LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
    DEPENDPATH += $$LOG4QT_LIB_DIR
}

# ---- macOS ----
macx {
    contains(QT_ARCH, arm64) {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/mac/arm64
    } else {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/mac/x86_64
    }
    LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
    PRE_TARGETDEPS += $$LOG4QT_LIB_DIR/liblog4qt.a
}

# ---- Linux ----
unix:!macx {
    contains(QT_ARCH, arm64)|contains(QT_ARCH, aarch64) {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/linux/arm64
    } else {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/linux/x86_64
    }
    LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
    PRE_TARGETDEPS += $$LOG4QT_LIB_DIR/liblog4qt.a
}

#-----------------------------------------------------------------------

# 链接前把 bin/ 下的运行时资源（log.conf 等）拷到 exe 输出目录
# main.cpp 通过 applicationDirPath()/log.conf 加载配置，必须与 exe 同目录
# 跨平台依赖：$$QMAKE_COPY 在 Windows 展开为 "copy /y"，Unix 展开为 "cp -f"
DEPLOY_FILES = $$files($$PWD/bin/*)
for(deploy_file, DEPLOY_FILES) {
    QMAKE_PRE_LINK += $${QMAKE_COPY} $$shell_quote($$shell_path($$deploy_file)) $$shell_quote($$shell_path($$OUT_PWD)) $$escape_expand(\\n\\t)
}

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
