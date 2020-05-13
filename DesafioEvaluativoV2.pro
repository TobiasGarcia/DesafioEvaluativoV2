TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        adminSession.cpp \
        code_decode.cpp \
        dialog.cpp \
        login.cpp \
        main.cpp \
        show.cpp \
        userSession.cpp

HEADERS += \
    adminSession.h \
    code_decode.h \
    dialog.h \
    login.h \
    show.h \
    userSession.h
