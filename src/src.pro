QT += \
    core \
    gui \
    widgets \
    network

CONFIG += c++17

win32: {
    # Add Windows series specific platform sources.
    SOURCES += sdk/knlockedfile_win.cpp
}

unix: {
    # Add UNIX series specific platform sources.
    SOURCES += sdk/knlockedfile_unix.cpp
}

INCLUDEPATH += \
    sdk

SOURCES += \
    main.cpp \
    sdk/knsingletonapplication.cpp \
    sdk/knlocalpeer.cpp \
    sdk/knlockedfile.cpp \
    ui/mainwindow.cpp

HEADERS += \
    sdk/knsingletonapplication.h \
    sdk/knlocalpeer.h \
    sdk/knlockedfile.h \
    ui/mainwindow.h
