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
#ifndef KNPLUGINMANAGER_H
#define KNPLUGINMANAGER_H

#include <QObject>

class KNMainWindow;
class KNAppChapterSelectorBase;
class KNMenuBase;
/*!
 * \brief The KNPluginManager class is the manager of all the classes for the
 * entire application. It manages all the plugins.
 */
class KNPluginManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPluginManager object.
     * \param parent The parent object.
     */
    explicit KNPluginManager(QObject *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Set the global main window.
     * \param mainWindow The main window pointer.
     */
    void setMainWindow(KNMainWindow *mainWindow);

    /*!
     * \brief Load all the plugins.
     */
    void loadPlugins();

    /*!
     * \brief Start the application.
     */
    void launchApplication();

    /*!
     * \brief Save all the configurations.
     */
    void saveConfigure();

private:
    inline void setApplicationInformation();
    void loadChapterSelector(KNAppChapterSelectorBase *chapterSelector);
    void loadLocationMenu(KNMenuBase *locationMenu);
    KNMainWindow *m_mainWindow;

    KNAppChapterSelectorBase *m_chapterSelector;
    KNMenuBase *m_locationMenu;
};

#endif // KNPLUGINMANAGER_H
