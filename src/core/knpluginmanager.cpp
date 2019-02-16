/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QStyleFactory>

#include "knversion.h"
#include "knmainwindow.h"
#include "knglobal.h"
#include "knconfiguremanager.h"

#include "knmenubase.h"

#include "plugin/knappchapterselector/knappchapterselector.h"
#include "plugin/knlocationmenu/knlocationmenu.h"

#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) : QObject(parent),
    m_mainWindow(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Set fusion style to application.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Initial the global.
    KNGlobal::initial(this);
}

void KNPluginManager::setMainWindow(KNMainWindow *mainWindow)
{
    if(m_mainWindow==nullptr)
    {
        //Save the main window object.
        m_mainWindow = mainWindow;
        //Link the main window to the plugin manager.
        connect(m_mainWindow, &KNMainWindow::requireSaveConfigure,
                this, &KNPluginManager::saveConfigure);
    }
}

void KNPluginManager::loadPlugins()
{
    m_mainWindow->appendAppWidget(new KNAppChapterSelector());
    m_mainWindow->setMenuWidget(new KNLocationMenu());
}

void KNPluginManager::launchApplication()
{
    //Show the main window.
    m_mainWindow->show();
}

void KNPluginManager::saveConfigure()
{
    //Save the configure, this must be done at last.
    knConf->saveConfigure();
}

inline void KNPluginManager::setApplicationInformation()
{
    //Generate the application name.
    QString applicationName("Cinema");
    //Set static application information.
    QApplication::setApplicationName(applicationName);
    QApplication::setApplicationVersion(APP_VERSION_STR);
#ifndef Q_OS_WIN
    QApplication::setApplicationDisplayName(applicationName);
#endif
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");
    //Configure application attributes.
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
}
