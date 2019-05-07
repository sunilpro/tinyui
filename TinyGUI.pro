QT += gui

CONFIG += c++11
//CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color.cpp \
    column.cpp \
    flickable.cpp \
    listview.cpp \
    mousearea.cpp \
    nanovg.c \
    cute.cpp \
    perf.c \
    item.cpp \
    object.cpp \
    rectangle.cpp \
    anchors.cpp \
    row.cpp \
    screen.cpp \
    examples/tagcloud.cpp \
    examples/director_admin.cpp \
    text.cpp

macx {
SOURCES += app_gl3.cpp
}
!macx {
SOURCES += app_wasm.cpp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    color.h \
    column.h \
    flickable.h \
    listview.h \
    mousearea.h \
    object.h \
    item.h \
    fontstash.h \
    nanovg_gl_utils.h \
    nanovg_gl.h \
    nanovg.h \
    row.h \
    screen.h \
    stb_image.h \
    stb_truetype.h \
    cute.h \
    stb_image_write.h \
    perf.h \
    rectangle.h \
    anchors.h \
    signal.hpp \
    property.hpp \
    text.h

INCLUDEPATH += /usr/local/opt/glfw/include

LIBS += -L/usr/local/opt/glfw/lib -lglfw -framework OpenGL
