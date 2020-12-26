TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

INCLUDEPATH += ../Librairies/OpenCV-4.0.1-build-debug-x64/include

LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_core401
LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_highgui401
LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_imgproc401
LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_imgcodecs401
LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_videoio401
LIBS += -L..\Librairies\OpenCV-4.0.1-build-debug-x64\lib/ -lopencv_features2d401

SOURCES += \
        Color.cpp \
        main.cpp

HEADERS += \
    Color.h
