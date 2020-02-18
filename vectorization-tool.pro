QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):

QT += widgets xml

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#QMAKE_CXXFLAGS += "-Wno-c++98-compat"

INCLUDEPATH += $$PWD/drawing_tools
INCLUDEPATH += $$PWD/entities

SOURCES += \
    DarkStyle.cpp \
    canvasscene.cpp \
    defaults.cpp \
    drawing_tools/drawingtool.cpp \
    drawing_tools/handmovingtool.cpp \
    drawing_tools/itemsmovingtool.cpp \
    drawing_tools/linedrawingtool.cpp \
    drawing_tools/pointsdrawingtool.cpp \
    drawing_tools/polygondrawingtool.cpp \
    drawing_tools/polylinedrawingtool.cpp \
    entities/entity.cpp \
    entities/image.cpp \
    entities/line.cpp \
    entities/point.cpp \
    entities/polygon.cpp \
    entities/polyline.cpp \
    drawingarea.cpp \
    drawingconfig.cpp \
    entitiesmanager.cpp \
    framelesswindow/framelesswindow.cpp \
    framelesswindow/windowdragger.cpp \
    highlighteffect.cpp \
    layeritem.cpp \
    layerstoolbar.cpp \
    main.cpp \
    mainwindow.cpp \
    topologymanager.cpp \
    utilities.cpp

HEADERS += \
    DarkStyle.h \
    canvasscene.h \
    defaults.h \
    drawing_tools/drawingtool.h \
    drawing_tools/handmovingtool.h \
    drawing_tools/itemsmovingtool.h \
    drawing_tools/linedrawingtool.h \
    drawing_tools/pointsdrawingtool.h \
    drawing_tools/polygondrawingtool.h \
    drawing_tools/polylinedrawingtool.h \
    entities/entity.h \
    entities/image.h \
    entities/line.h \
    entities/point.h \
    entities/polygon.h \
    entities/polyline.h \
    drawingarea.h \
    drawingconfig.h \
    entitiesmanager.h \
    framelesswindow/framelesswindow.h \
    framelesswindow/windowdragger.h \
    highlighteffect.h \
    layeritem.h \
    layerstoolbar.h \
    mainwindow.h \
    topologymanager.h \
    utilities.h

FORMS += \
    framelesswindow/framelesswindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    drawing_tools/drawing_tools.pri \
    entities/entities.pri \
    images/esi-logo.ico

RESOURCES += \
    framelesswindow.qrc \
    darkstyle.qrc

RC_ICONS = images/esi-logo.ico
