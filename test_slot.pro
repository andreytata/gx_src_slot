QT += core gui widgets websockets

CONFIG += c++11
CONFIG += console

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gx_src_es20_root.cpp \
        main.cpp \
    gx_src_slot.cpp \
    gx_src_type.cpp \
    gx_src_load.cpp \
    gx_src_serv.cpp \
    gx_src_sess.cpp \
    gx_src_es20_prog.cpp \
    gx_src_es20_shgr.cpp \
    gx_src_es20_unfa.cpp \
    gx_src_es20_vtxa.cpp \
    gx_src_self.cpp \
    gx_src_gxvm.cpp \
    gx_src_gxvm_seq.cpp \
    gx_src_gxvm_vip.cpp

HEADERS += \
    gx_src_es20.h \
    gx_src_slot.h \
    gx_src_type.h \
    gx_src_test.h \
    gx_src_serv.h \
    gx_src_es20_vtxa.h \
    gx_src_es20_unfa.h \
    gx_src_user.h \
    gx_src_gxvm.h
