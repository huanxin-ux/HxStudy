#QT       += core

CONFIG += c++14
TEMPLATE = lib

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $$PWD/bin

INCLUDEPATH += /home/huanxin/hx/tool/opendds/ACE_wrappers \
               /home/huanxin/hx/tool/opendds/ACE_wrappers/TAO \
               /home/huanxin/hx/tool/opendds/OpenDDS-3.26.1 \
               $$PWD/build/opendds_generated


LIBS += -L/home/huanxin/hx/tool/opendds/ACE_wrappers/lib \
        -L/home/huanxin/hx/tool/opendds/OpenDDS-3.26.1/lib -L$$PWD/bin \
        -lDDS_IDL_idl -lOpenDDS_Dcps -lOpenDDS_Tcp -lOpenDDS_InfoRepoDiscovery \
        -lACE -lTAO -lTAO_AnyTypeCode -lTAO_PortableServer -lTAO_CodecFactory \
        -lTAO_PI -lTAO_BiDirGIOP -lTAO_Valuetype

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DDSService.cpp \
    DataReaderListenerImpl.cpp \
    Boilerplate.cpp

HEADERS += \
    DDSService.h \
    DataReaderListenerImpl.h \
    Boilerplate.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
