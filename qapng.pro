TEMPLATE = lib
CONFIG += plugin

QT       += core
LIBS += -llibpng16 -lzlib
greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}
contains(TARGET_ARCH, x86_64) {
    TARGET_ARCH = x64
} else {
    TARGET_ARCH = x86
}

win32 {
  CONFIG(debug, debug|release) {
message("libpng debug")
    LIBS += -LE:\Home\src\opensource\clang\libpng\projects\vstudio/$${TARGET_ARCH}/Debug
  } else {
message("libpng release")
    LIBS += -LE:\Home\src\opensource\clang\libpng\projects\vstudio/$${TARGET_ARCH}/Release
  }
}

INCLUDEPATH += E:\Home\src\opensource\clang\libpng  E:\Home\src\opensource\clang\zlib-1.2.8

TARGET = $$qtLibraryTarget(qapng)
VERSION = 2.0.0

HEADERS += apngimageplugin.h \
	apngimagehandler.h \
	loadapng.h

SOURCES += apngimageplugin.cpp \
	apngimagehandler.cpp \
	loadapng.cpp

DISTFILES += qapng.json

target.path =  $$[QT_INSTALL_PLUGINS]/imageformats
INSTALLS += target
