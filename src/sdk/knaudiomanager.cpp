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
#include <QDir>

#include "bass.h"
#include "knutil.h"

#include "knaudiomanager.h"

KNAudioManager *KNAudioManager::ins=nullptr;

KNAudioManager *KNAudioManager::instance()
{
    return ins;
}

void KNAudioManager::initial(QObject *parent)
{
    //Check the instance first.
    if(ins==nullptr)
    {
        ins=new KNAudioManager(parent);
    }
}

void KNAudioManager::setAudioDirectory(const QString &audioDirPath)
{
    //Set the audio dir path.
    m_audioDirectory=QDir(KNUtil::ensurePathValid(audioDirPath));
    //Set the directory.
    for(int i=0; i<AudioTypeCount; ++i)
    {
        //Try to load the file.
        QString filePath=m_audioDirectory.filePath(m_fileName.at(i));
        #ifdef Q_OS_WIN
            std::wstring uniPath=filePath.toStdWString();
        #endif
        #ifdef Q_OS_UNIX
            std::string uniPath=filePath.toStdString();
        #endif
        //Load all the files.
        m_audioChannel[i]=BASS_StreamCreateFile(FALSE, uniPath.data(), 0, 0, 0);
    }
}

KNAudioManager::KNAudioManager(QObject *parent) : QObject(parent),
    m_audioDirectory(QDir())
{
    //Initial the bass.
    // enable "Default" device that follows default device changes
    BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);
    // initialize default output device
    BASS_Init(-1, 44100, 0, nullptr, nullptr);
    //Load and set.
    m_fileName.reserve(AudioTypeCount);
    m_fileName << "startup.wav" << "move.wav" << "ok.wav" << "cancel.wav"
               << "error.wav" << "menu_open.wav" << "menu_close.wav";
}

void KNAudioManager::play(int type)
{
    //Check the type.
    if(type >= m_fileName.size())
    {
        //Invalid type.
        return;
    }
    //Check the audio player current playing status.
    //Play the thread.
    BASS_ChannelPlay(m_audioChannel[type], TRUE);
}
