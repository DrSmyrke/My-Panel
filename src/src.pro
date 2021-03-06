#-------------------------------------------------
#
# Project created by QtCreator 2017-12-27T16:39:08
#
#-------------------------------------------------

QT       += core gui network x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mypanel
TEMPLATE = app

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

CONFIG    += link_pkgconfig
PKGCONFIG += x11
CONFIG += c++11

CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
#    unix: TARGET = $${TARGET}_debug
#    else: TARGET = $${TARGET}d
}

OBJECTS_DIR         = ../build/obj/$${CONFIGURATION}
MOC_DIR             = ../build/$${CONFIGURATION}
RCC_DIR             = ../build/rcc
UI_DIR              = ../build/ui
DESTDIR             = ../bin

win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}
QMAKE_CXXFLAGS += "-std=c++11"

SOURCES += main.cpp\
        mainwindow.cpp \
    global.cpp \
    nativeeventfilter.cpp \
    windows/settings.cpp \
    windows/execwindow.cpp \
    windows/bookmarkswindow.cpp \
    wget.cpp \
    dataupdate.cpp \
    myfunctions.cpp \
    windows/auth.cpp

HEADERS  += mainwindow.h \
    global.h \
    nativeeventfilter.h \
    windows/settings.h \
    windows/execwindow.h \
    windows/bookmarkswindow.h \
    wget.h \
    dataupdate.h \
    myfunctions.h \
    windows/auth.h

TRANSLATIONS = lang/ru_RU.ts

RESOURCES += \
    resources.qrc

exists(./gitversion.pri):include(./gitversion.pri)
exists(./myLibs.pri):include(./myLibs.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
