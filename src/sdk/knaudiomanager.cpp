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
#include <QMediaPlayer>
#include <QDir>

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
}

KNAudioManager::KNAudioManager(QObject *parent) : QObject(parent),
    m_audioDirectory(QDir()),
    m_audioPlayer(new QMediaPlayer(this))
{
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
    if(m_audioPlayer->state() != QMediaPlayer::StoppedState)
    {
        //Stop the current playing audio.
        m_audioPlayer->stop();
    }
    //Set the media content according to the type.
    m_audioPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(
                                              m_audioDirectory.filePath(
                                                  m_fileName.at(type)))));
    //Play the audio.
    m_audioPlayer->play();
}
