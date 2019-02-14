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
#include <QLabel>
#include <QVideoWidget>
#include <QMovie>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>

#include "knbackground.h"

#include <QDebug>

KNBackground::KNBackground(QWidget *parent) :
    QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_videoDisplay(new QGraphicsVideoItem()),
    m_imageDisplay(new QLabel(this)),
    m_videoLoopList(new QMediaPlaylist(this)),
    m_videoPlayer(new QMediaPlayer(this))
{
    //Configure the view.
    setScene(m_scene);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Add widget to scene.
    m_scene->addItem(m_videoDisplay);
    //Set video player attributes.
    m_videoPlayer->setAudioRole(QAudio::VideoRole);
    m_videoPlayer->setPlaylist(m_videoLoopList);
    m_videoPlayer->setVideoOutput(m_videoDisplay);
    m_videoLoopList->setPlaybackMode(QMediaPlaylist::Loop);
    //Hide the display.
    m_videoDisplay->hide();
    //Set image player attributes.
    m_imageDisplay->setFocusProxy(this);
    m_imageDisplay->setScaledContents(true);

    // Load default background.
    m_imageDisplay->setPixmap(QPixmap(":/background.png"));
}

void KNBackground::setBackground(const QPixmap &image)
{
    //Check the image validation.
    if(image.isNull())
    {
        return;
    }
    //Set the background as the image player.
    m_imageDisplay->setPixmap(image);
    //Show the image player.
    m_imageDisplay->show();
    m_videoDisplay->hide();
    //Update the display.
    update();
}

void KNBackground::setBackground(const QString &videoPath)
{
    //Stop the video player.
    if(m_videoPlayer->state()!=QMediaPlayer::StoppedState)
    {
        m_videoPlayer->stop();
    }
    //Set the background as a video.
    QMediaContent backgroundVideo(QUrl::fromLocalFile(videoPath));
    if(!backgroundVideo.isNull())
    {
        //Update the video loop list.
        m_videoLoopList->clear();
        m_videoLoopList->addMedia(backgroundVideo);
        //Play the new video list.
        m_videoPlayer->play();
        //Show the video display.
        m_videoDisplay->show();
        m_imageDisplay->hide();
    }
}

void KNBackground::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Resize the both widget.
    m_videoDisplay->setSize(QSize(width(), height()));
    m_imageDisplay->resize(width(), height());
}
