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
#include <QStandardPaths>

#include "knutil.h"
#include "knconfigure.h"
#include "knconfiguremanager.h"
#include "knfontmanager.h"
#include "kndpimanager.h"
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knaudiomanager.h"

#include "knglobal.h"

//Initial the instance pointer to null.
KNGlobal *KNGlobal::ins=nullptr;

KNGlobal *KNGlobal::instance()
{
    //Return the instance pointer.
    return ins;
}

void KNGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(ins==nullptr)
    {
        ins=new KNGlobal(parent);
    }
}

QString KNGlobal::dirPath(int index)
{
    Q_ASSERT(index>-1 && index<DefaultDirCount);
    return m_dirPath[index];
}

KNConfigure *KNGlobal::cacheConfigure()
{
    return knConf->configure(KNConfigureManager::Cache);
}

KNConfigure *KNGlobal::systemConfigure()
{
    return knConf->configure(KNConfigureManager::System);
}

KNConfigure *KNGlobal::userConfigure()
{
    return knConf->configure(KNConfigureManager::User);
}

KNGlobal::KNGlobal(QObject *parent) : QObject(parent),
    m_globalConfigure(nullptr)
{
    //Initial the managers.
    //Gerenate the configure manager.
    KNConfigureManager::initial(this);
    //Generate the DPI manager.
    KNDpiManager::initial(this);
    //Generate the font manager.
    KNFontManager::initial(this);
    //Generate the locale manager.
    KNLocaleManager::initial(this);
    //Generate the theme manager.
    KNThemeManager::initial(this);
    //Generate the audio manager.
    KNAudioManager::initial(this);

    //Initial the infrastructure.
    initialInfrastrcture();
    //Update the infrastructure.
    // Load the language.
    knI18n->setLanguage(m_globalConfigure->data("Language").toString());
}

void KNGlobal::initialDefaultDirPath()
{
    /*
     * Initial the default path of the dirs.
     * Kreogist Dir:
     *     Windows: My documents/Kreogist/
     *    Mac OS X: Documents/Kreogist/
     *       Linux: ~/.kreogist/
     * User Data Dir:
     *     Windows: My documents/Kreogist/Cinema
     *    Mac OS X: Documents/Kreogist/Cinema
     *       Linux: ~/.kreogist/cinema
     * Resource Dir:
     *     Windows: Application path
     *    Mac OS X: Application path/../Resources
     *       Linux: ~/.kreogist/cinema
     * Library Dir:
     *      $UserDataDir$/Library
     * Plugin Dir:
     *      $UserDataDir$/Plugins
     */
#ifdef Q_OS_WIN //No matter Win32/Win64
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::DocumentsLocation)
                                   +"/Kreogist");
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/Cinema";
    m_dirPath[ResourceDir]=qApp->applicationDirPath();
#endif
#ifdef Q_OS_MACX
    m_dirPath[KreogistDir]=
                KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                           QStandardPaths::DocumentsLocation)
                                       +"/Kreogist");
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/Cinema";
    m_dirPath[ResourceDir]=
            KNUtil::simplifiedPath(qApp->applicationDirPath()+"/../Resources");
#endif
#ifdef Q_OS_LINUX
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::HomeLocation))
            + "/.kreogist";
    m_dirPath[UserDataDir]=m_dirPath[KreogistDir]+"/cinema";
    m_dirPath[ResourceDir]=m_dirPath[UserDataDir];
#endif
    m_dirPath[LibraryDir]=
            KNUtil::simplifiedPath(m_dirPath[UserDataDir]+"/Library");
}

void KNGlobal::initialInfrastrcture()
{
    //Initial the default path.
    initialDefaultDirPath();

    //Initial the configure manager.
    //Set the configure folder path.
    //-- Q: Why set configure path here?
    //   A: Because we won't change the configure path.
    //Because KNConfigureManager will automatically reload the configure, so we
    //don't need to load the configure here.
    knConf->setFolderPath(m_dirPath[UserDataDir]+"/Configure");
    //Get the global configure.
    m_globalConfigure=userConfigure()->getConfigure("Global");

    //Initial the font manager.
    //Set the font resource folder.
    knFont->loadCustomFontFolder(m_dirPath[ResourceDir]+"/Fonts");
    //Set the font configure to the font manager.
    knFont->setConfigure(m_globalConfigure);
    //Set the default font.
    knFont->loadDefaultFont();

    //Initial the locale manager.
    //Load the language in language folder.
#ifdef Q_OS_LINUX
    //Thanks for Sou Bunnbu, amazingfate:
    //For Linux, we should also find langauges at /usr/share/Kreogist/cinema.
    knI18n->addLanguageDirectory("/usr/share/Kreogist/cinema/Language");
    //And the user installed languages, thanks for 1dot75cm.
    knI18n->addLanguageDirectory(m_dirPath[UserDataDir]+"/Language");
    knI18n->loadLanguageFiles();
#else
    //For Windows and Mac OS X, we can simply check the resource folder and
    //application dir.
    knI18n->addLanguageDirectory(m_dirPath[ResourceDir]+"/Language");
    knI18n->addLanguageDirectory(qApp->applicationDirPath()+"/Language");
    knI18n->loadLanguageFiles();
#endif
    //Load the current language file.
    //We will load the langauge file twice, for the first time, we have to load
    //the default language according to the system locale settings.
    knI18n->setDefaultLanguage();
    //Load the language stored in the configure file.
    knI18n->setLanguage(m_globalConfigure->data("Language").toString());

    //Initial the theme manager.
    //Load the theme in theme folder. It's familiar with the language folder.
#ifdef Q_OS_LINUX
    knTheme->loadThemeFiles("/usr/share/Kreogist/cinema/Theme");
#else
    knTheme->loadThemeFiles(m_dirPath[ResourceDir]+"/Theme");
#endif
    //Load the current theme file.
    //Like the language, we will load theme twice. Default first, user next.
    knTheme->setTheme(0);
    //Load the theme in the configure file.
    knTheme->setTheme(m_globalConfigure->data("Theme").toString());

    //Initial the audio manager.
    //Set the audio directory path.
    knAudio->setAudioDirectory(m_dirPath[ResourceDir]+"/Audio");
}
