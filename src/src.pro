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
    sdk/knaudiomanager.cpp \
    sdk/knbackground.cpp \
    sdk/knmainapplicationheader.cpp \
    sdk/kntextclock.cpp \
    sdk/knhwidgetswitcher.cpp \
    sdk/knwidgetswitcher.cpp \
    plugin/knappchapterselector/knchaptermodel.cpp \
    plugin/knappchapterselector/knproxychaptermodel.cpp \
    plugin/knappchapterselector/knchaptersearcher.cpp \
    plugin/knappchapterselector/knchapterutil.cpp \
    sdk/knmenulayer.cpp \
    sdk/knclocklabel.cpp \
    sdk/knsaostyle.cpp \
    sdk/knscrollarea.cpp \
    plugin/knlocationmenu/knlocationmenu.cpp \
    plugin/knappchapterselector/knchapterselector.cpp \
    plugin/knappchapterselector/knappchapterselector.cpp

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
    sdk/knaudiomanager.h \
    sdk/knbackground.h \
    sdk/knmainapplicationheader.h \
    sdk/kntextclock.h \
    sdk/knhwidgetswitcher.h \
    sdk/knwidgetswitcher.h \
    plugin/knappchapterselector/knchaptermodel.h \
    plugin/knappchapterselector/knproxychaptermodel.h \
    plugin/knappchapterselector/knchaptersearcher.h \
    plugin/knappchapterselector/knchapterutil.h \
    sdk/knmenulayer.h \
    sdk/knclocklabel.h \
    sdk/knappchapterselectorbase.h \
    sdk/knsaostyle.h \
    sdk/knscrollarea.h \
    plugin/knlocationmenu/knlocationmenu.h \
    plugin/knappchapterselector/knchapterselector.h \
    plugin/knappchapterselector/knappchapterselector.h \
    sdk/knmenubase.h

RESOURCES += \
    resource/resource.qrc
