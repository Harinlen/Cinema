QT += \
    core \
    gui \
    widgets \
    network \
    multimedia \
    multimediawidgets

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
    core/knpluginmanager.cpp \
    sdk/knmainwindow.cpp \
    sdk/knglobal.cpp \
    sdk/knconfigure.cpp \
    sdk/knconfiguremanager.cpp \
    sdk/knfontmanager.cpp \
    sdk/knlocalemanager.cpp \
    sdk/knthememanager.cpp \
    sdk/kndpimanager.cpp \
    sdk/knutil.cpp \
    sdk/knapplicationlayer.cpp \
    sdk/knbootlayer.cpp \
    sdk/knaudiomanager.cpp

HEADERS += \
    sdk/knsingletonapplication.h \
    sdk/knlocalpeer.h \
    sdk/knlockedfile.h \
    core/knpluginmanager.h \
    sdk/knmainwindow.h \
    sdk/knglobal.h \
    sdk/knconfigure.h \
    sdk/knconfiguremanager.h \
    sdk/knfontmanager.h \
    sdk/knlocalemanager.h \
    sdk/knthememanager.h \
    sdk/kndpimanager.h \
    sdk/knutil.h \
    sdk/knversion.h \
    sdk/knapplicationlayer.h \
    sdk/knbootlayer.h \
    sdk/knaudiomanager.h

RESOURCES += \
    resource/resource.qrc
