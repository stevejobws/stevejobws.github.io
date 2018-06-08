#-------------------------------------------------
#
# Project created by QtCreator 2017-07-10T08:58:51
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += printsupport
QT       += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Home
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    yeelink/yeelinkapi_v1.cpp \
    qcustomplot/qcustomplot.cpp \
    qcom/qextserialbase.cpp \
    qcom/win_qextserialport.cpp \
    opencv/dealpicture.cpp \
    opencv/imagetomat.cpp \
    opencv/mattoimage.cpp \
    opencv/qpixmaptoqimage.cpp

HEADERS  += widget.h \
    yeelink/yeelinkapi_v1.h \
    qcom/qextserialbase.h \
    qcom/win_qextserialport.h \
    qcustomplot.h \
    opencv/dealpicture.h \
    opencv/imagetomat.h \
    opencv/mattoimage.h \
    opencv/qpixmaptoqimage.h

FORMS    += widget.ui

DISTFILES +=

RESOURCES += \
    image.qrc

INCLUDEPATH+=E:\opencv_MinGW32\install\include\opencv\
                    E:\opencv_MinGW32\install\include\opencv2\
                  E:\opencv_MinGW32\install\include\

LIBS += E:\opencv_MinGW32\lib\lib*
LIBS += E:\opencv_MinGW32\lib\libopencv_core248.dll.a
LIBS += E:\opencv_MinGW32\lib\libopencv_highgui248.dll.a
LIBS += E:\opencv_MinGW32\lib\libopencv_imgproc248.dll.a
LIBS += E:\opencv_MinGW32\lib\libopencv_objdetect248.dll.a
