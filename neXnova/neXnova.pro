#-------------------------------------------------
#
# Project created by QtCreator 2018-06-16T21:16:47
#
#-------------------------------------------------

QT += widgets
QT += core gui network qml quick quickwidgets multimedia quickcontrols2
QT += 3dcore 3drender 3dinput 3dlogic 3dextras 3dquick 3danimation 3dquickextras

CONFIG += link_pkgconfig
PKGCONFIG += opus opencv libswscale libswresample libavcodec libavutil# x264

DEFINES += OPUS

TARGET = neXnova
TEMPLATE = app

QML_IMPORT_PATH += /opt/Qt5.11.0/5.11.0/gcc_64/qml

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    connection.cpp \
    qmlengine.cpp \
    audioengine.cpp \
    3d/solarsistem.cpp \
    3d/planet.cpp \
    3d/ring.cpp \
    3d/moon.cpp \
    3d/meteor.cpp \
    3d/spacestation.cpp \
    3d/star.cpp \
    3d/skybox.cpp \
    3d/customrender.cpp \
    3d/controledcamera.cpp \
    3d/universe.cpp \
    razeselector.cpp \
    chatclientsmodel.cpp \
    imageprovider.cpp \
    qmlpluguins/circleimage.cpp \
    rotation.cpp \
    jsoncachelayer.cpp \
    audioengine/audioinput.cpp \
    audioengine/audiooutput.cpp \
    audioengine/client.cpp \
    audioengine/opusdecode.cpp \
    audioengine/opusencode.cpp \
    audioengine/r8brain.cpp \
    audioengine/server.cpp \
    audioengine/r8brain/r8bbase.cpp \
    videochat.cpp \
    planetbarmodel.cpp \
    qmlpluguins/fps.cpp \
    qmlpluguins/videowidget.cpp \
    qmlpluguins/techtree.cpp

HEADERS += \
    connection.h \
    qmlengine.h \
    audioengine.h \
    3d/solarsistem.h \
    3d/planet.h \
    3d/ring.h \
    3d/moon.h \
    3d/meteor.h \
    3d/spacestation.h \
    3d/star.h \
    3d/skybox.h \
    3d/customrender.h \
    3d/controledcamera.h \
    3d/universe.h \
    razeselector.h \
    chatclientsmodel.h \
    imageprovider.h \
    qmlpluguins/circleimage.h \
    rotation.h \
    jsoncachelayer.h \
    audioengine/audioinput.h \
    audioengine/audiooutput.h \
    audioengine/client.h \
    audioengine/opusdecode.h \
    audioengine/opusencode.h \
    audioengine/r8brain.h \
    audioengine/server.h \
    audioengine/r8brain/CDSPBlockConvolver.h \
    audioengine/r8brain/CDSPFIRFilter.h \
    audioengine/r8brain/CDSPFracInterpolator.h \
    audioengine/r8brain/CDSPProcessor.h \
    audioengine/r8brain/CDSPRealFFT.h \
    audioengine/r8brain/CDSPResampler.h \
    audioengine/r8brain/CDSPSincFilterGen.h \
    audioengine/r8brain/fft4g.h \
    audioengine/r8brain/r8bbase.h \
    audioengine/r8brain/r8bconf.h \
    audioengine/r8brain/r8butil.h \
    videochat.h \
    planetbarmodel.h \
    qmlpluguins/fps.h \
    qmlpluguins/videowidget.h \
    qmlpluguins/techtree.h


RESOURCES += \
    assets.qrc \
    qml.qrc \
    gui.qrc \
    sfx.qrc \
    assets2.qrc

DISTFILES += \
    main.py \
    assets/shaders/gl3/sunx.frag \
    assets/shaders/gl3/sunx.vert

target.path = /home/hoax/nexwork
INSTALLS += target
