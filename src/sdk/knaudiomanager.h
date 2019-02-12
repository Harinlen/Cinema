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
#ifndef KNAUDIOENGINE_H
#define KNAUDIOENGINE_H

#include <QDir>

#include <QObject>

#define knAudio (KNAudioManager::instance())

class QMediaPlayer;
/*!
 * \brief The KNAudioEngine class provides an audio engine for playing hint
 * sounds. All the sounds will be dynamicly load from the runtime directory.
 */
class KNAudioManager : public QObject
{
    Q_OBJECT
public:
    enum AudioType
    {
        AudioStartUp,
        AudioMove,
        AudioOk,
        AudioCancel,
        AudioError,
        AudioMenuOpen,
        AudioMenuClose,
        AudioTypeCount
    };

    /*!
     * \brief Get the instance of the audio manager.
     * \return The instance of the audio manager.
     */
    static KNAudioManager *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Set the audio directory path.
     * \param audioDirPath The target dir path.
     */
    void setAudioDirectory(const QString &audioDirPath);

    /*!
     * \brief Play the target audio according to the type.
     * \param type The audio type.
     */
    void play(int type);

private:
    static KNAudioManager *ins;
    explicit KNAudioManager(QObject *parent = nullptr);
    KNAudioManager(const KNAudioManager &);
    KNAudioManager(KNAudioManager &&);

    QStringList m_fileName;
    QDir m_audioDirectory;
    QMediaPlayer *m_audioPlayer;

};

#endif // KNAUDIOENGINE_H
