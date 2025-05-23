QT -= gui

CONFIG += c++11
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH    += /usr/include/gstreamer-1.0/ \
                  /usr/include/glib-2.0 \
                  /usr/lib/x86_64-linux-gnu/glib-2.0/include \
                  /usr/lib/gstreamer-1.0/gst \

LIBS += -lgstrtspserver-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
