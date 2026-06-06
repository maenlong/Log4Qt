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
    LOG4QT_SDK_FILE = $$LOG4QT_LIB_DIR/log4qt.lib
    exists($$LOG4QT_SDK_FILE) {
        message("[log4qt] found library: $$LOG4QT_SDK_FILE")
        DEFINES += HAVE_LOG4QT
        LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
        DEPENDPATH += $$LOG4QT_LIB_DIR
    } else {
        warning("[log4qt] LIBRARY NOT FOUND: $$LOG4QT_SDK_FILE")
        warning("[log4qt] HAVE_LOG4QT not defined; log subsystem will be unavailable")
    }
}

# ---- macOS ----
macx {
    contains(QT_ARCH, arm64) {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/mac/arm64
    } else {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/mac/x86_64
    }
    LOG4QT_SDK_FILE = $$LOG4QT_LIB_DIR/liblog4qt.a
    exists($$LOG4QT_SDK_FILE) {
        message("[log4qt] found library: $$LOG4QT_SDK_FILE")
        DEFINES += HAVE_LOG4QT
        LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
        PRE_TARGETDEPS += $$LOG4QT_SDK_FILE
    } else {
        warning("[log4qt] LIBRARY NOT FOUND: $$LOG4QT_SDK_FILE")
        warning("[log4qt] HAVE_LOG4QT not defined; log subsystem will be unavailable")
    }
}

# ---- Linux ----
unix:!macx {
    contains(QT_ARCH, arm64)|contains(QT_ARCH, aarch64) {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/linux/arm64
    } else {
        LOG4QT_LIB_DIR = $$PWD/log4qtlib/lib/linux/x86_64
    }
    LOG4QT_SDK_FILE = $$LOG4QT_LIB_DIR/liblog4qt.a
    exists($$LOG4QT_SDK_FILE) {
        message("[log4qt] found library: $$LOG4QT_SDK_FILE")
        DEFINES += HAVE_LOG4QT
        LIBS += -L$$LOG4QT_LIB_DIR -llog4qt
        PRE_TARGETDEPS += $$LOG4QT_SDK_FILE
    } else {
        warning("[log4qt] LIBRARY NOT FOUND: $$LOG4QT_SDK_FILE")
        warning("[log4qt] HAVE_LOG4QT not defined; log subsystem will be unavailable")
    }
}

#-----------------------------------------------------------------------

# 链接后递归拷贝 bin/ 到 exe 同目录（macOS 进 .app/Contents/MacOS）
contains(DEFINES, HAVE_LOG4QT) {
    macx: DEPLOY_DST = $$OUT_PWD/$${TARGET}.app/Contents/MacOS
    else: DEPLOY_DST = $$OUT_PWD

    message("[log4qt] deploy bin/ contents -> $$DEPLOY_DST")

    win32 {
        QMAKE_POST_LINK += xcopy /S /Q /Y /I $$shell_quote($$shell_path($$PWD/bin)) $$shell_quote($$shell_path($$DEPLOY_DST)) $$escape_expand(\\n\\t)
    } else {
        # bin/. 语法拷贝内容（而非 bin 目录本身）
        QMAKE_POST_LINK += cp -fR $$shell_quote($$PWD/bin/.) $$shell_quote($$DEPLOY_DST/) $$escape_expand(\\n\\t)
    }
} else {
    warning("[log4qt] HAVE_LOG4QT not set; skipping bin/ deployment")
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
