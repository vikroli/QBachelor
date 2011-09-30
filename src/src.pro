SOURCES += main.cpp \
           mainwindow.cpp \
           studycourse.cpp \
           licensedialog.cpp \
           aboutdialog.cpp
TEMPLATE = app
CONFIG += warn_on \
          thread \
          qt \
          exceptions \
          release
TARGET = ../bin/qbachelor
RESOURCES = application.qrc
FORMS += mainwindow.ui \
         licensedialog.ui \
         aboutdialog.ui
HEADERS += mainwindow.h \
           studycourse.h \
           licensedialog.h \
           aboutdialog.h
QT += xml
CONFIG -= debug
win32 {
    RC_FILE = qbachelor.rc
    TARGET = .\..\..\bin\QBachelor
    INCLUDEPATH += .
}
linux {
    TARGET = ../bin/qbachelor
}
TRANSLATIONS += qbachelor_de.ts qbachelor_es.ts
