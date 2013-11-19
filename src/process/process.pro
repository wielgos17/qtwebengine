TARGET = $$QTWEBENGINEPROCESS_NAME
TEMPLATE = app

macx {
    LIBPATH = $$getOutDir()/$$getConfigDir()
    CONFIG -= app_bundle
} else:LIBPATH = $$getOutDir()/$$getConfigDir()/lib
LIBS_PRIVATE += -lQt5WebEngineCore -L$$LIBPATH
QMAKE_RPATHDIR += $$LIBPATH

qnx: QMAKE_RPATHLINKDIR += $${QNX_DIR}/$${QNX_CPUDIR}/usr/lib/qt5/lib

DESTDIR = $$getOutDir()/$$getConfigDir()

INCLUDEPATH += ../lib

SOURCES = main.cpp

target.files = $$DESTDIR/$$QTWEBENGINEPROCESS_NAME
target.path = $$[QT_INSTALL_LIBEXECS]
INSTALLS += target