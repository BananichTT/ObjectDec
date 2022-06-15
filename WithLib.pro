QT       += core gui testlib sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    test.cpp

HEADERS += \
    mainwindow.h \
    test.h

INCLUDEPATH += D:\opencv\build\install\include

LIBS += D:\opencv\build\bin\libopencv_core454.dll
LIBS += D:\opencv\build\bin\libopencv_highgui454.dll
LIBS += D:\opencv\build\bin\libopencv_imgcodecs454.dll
LIBS += D:\opencv\build\bin\libopencv_imgproc454.dll
LIBS += D:\opencv\build\bin\libopencv_features2d454.dll
LIBS += D:\opencv\build\bin\libopencv_calib3d454.dll
LIBS += D:\opencv\build\bin\libopencv_video454.dll
LIBS += D:\opencv\build\bin\libopencv_videoio454.dll

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../DetectorLib/build-DetectorLib-Desktop_Qt_6_2_4_MinGW_64_bit-Release/release/ -lDetectorLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../DetectorLib/build-DetectorLib-Desktop_Qt_6_2_4_MinGW_64_bit-Release/debug/ -lDetectorLib

INCLUDEPATH += $$PWD/../../DetectorLib/DetectorLib
DEPENDPATH += $$PWD/../../DetectorLib/DetectorLib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-LibDB-Desktop_Qt_6_2_4_MinGW_64_bit-Release/release/ -lLibDB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-LibDB-Desktop_Qt_6_2_4_MinGW_64_bit-Release/debug/ -lLibDB

INCLUDEPATH += $$PWD/../../LibDB
DEPENDPATH += $$PWD/../../LibDB
