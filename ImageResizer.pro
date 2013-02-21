#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T22:41:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageResizer
TEMPLATE = app


SOURCES += main.cpp\
        resizedialog.cpp \
    libs.cpp \
    imageresizer.cpp \
    show.cpp

HEADERS  += resizedialog.h \
    libs.h \
    imageresizer.h \
    show.h

FORMS    += resizedialog.ui \
    show.ui

win32 {
            INCLUDEPATH+="D:/Program Files/opencv/build/include/"
            INCLUDEPATH+="D:/Program Files/qwt/include"
            #QMAKE_LFLAGS += -static-libgcc -static-libstdc++
            LIBS+="D:/Program Files/opencv/build/x86/mingw/lib/libopencv_core242.dll.a"
            LIBS+="D:/Program Files/opencv/build/x86/mingw/lib/libopencv_highgui242.dll.a"
            LIBS+="D:/Program Files/opencv/build/x86/mingw/lib/libopencv_imgproc242.dll.a"
            DEPENDPATH+="D:/Program Files/opencv/build/x86/mingw/bin"
            DEPENDPATH+="D:/Program Files/qwt/lib/"
            DEPENDPATH+="D:/QtSDK/Desktop/Qt/4.8.1/mingw/bin"
}

unix {
            #INCLUDEPATH+=/usr/include/qwt
            LIBS+= -static-libgcc -static-libstdc++ -lGLU -lopencv_core -lopencv_highgui -lopencv_imgproc #-lqwt
}

RESOURCES += \
    icon.qrc
