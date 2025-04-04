QT       += core gui
QT += network
QMAKE_CFLAGS += -Wa,-mbig-obj
QMAKE_CXXFLAGS += -Wa,-mbig-obj
QT += charts
QT += widgets opengl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


CONFIG += c++17


win32 {
    LIBS += -L$$PWD/lib -llibcustomplotlib
} else {
    LIBS += -L$$PWD/lib -llibcustomplotlib
}

#INCLUDEPATH += $$PWD/lib
#DEPENDPATH += $$PWD/lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# INCLUDEPATH += include/
# VPATH += src/
SOURCES += \
    main.cpp \
    src/DataProcessor.cpp \
    src/FileWriter.cpp \
    src/Plotter.cpp \
    src/UDPReceiver.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/DataProcessor.h \
    src/FileWriter.h \
    src/Plotter.h \
    src/UDPReceiver.h \
    src/data.h \
    src/mainwindow.h


FORMS += \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

