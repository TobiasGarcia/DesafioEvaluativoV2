TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        code_decode.cpp \
        dialog.cpp \
        login.cpp \
        main.cpp \
        session.cpp \
        show.cpp

HEADERS += \
    code_decode.h \
    dialog.h \
    login.h \
    session.h \
    show.h
