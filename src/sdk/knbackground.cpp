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
#include <QPainter>
#include <QDir>
#include <QTimer>

#include "knbackground.h"

#include <QDebug>

KNBackground::KNBackground(QWidget *parent) :
    QWidget(parent),
    m_defaultBackground(QPixmap(":/background.png")),
    m_videoTimer(new QTimer(this)),
    m_frameIndex(-1)
{
    //Configure the timer.
    m_videoTimer->setInterval(32);
    connect(m_videoTimer, &QTimer::timeout, [=]
    {
        //Update the frame index.
        ++m_frameIndex;
        //Check up bound.
        if(m_frameIndex>=m_frameNameList.size())
        {
            //Reset it back to beginning.
            m_frameIndex=0;
        }
        if(!m_scaledMap.contains(m_frameIndex))
        {
            m_scaledMap.insert(m_frameIndex,
                               QPixmap(m_frameNameList.at(m_frameIndex)).scaled(
                                   width(), height()));
        }
        //Load the frame and scale it.
        m_scaledFrame=m_scaledMap.value(m_frameIndex);
        //Update the widget.
        update();
    });
    //Load default background.
    setBackground(m_defaultBackground);
}

void KNBackground::setBackground(const QPixmap &image)
{
    //Check the image validation.
    if(image.isNull())
    {
        return;
    }
    //Clear the stream.
    m_videoTimer->stop();
    m_frame=image;
    //Reset the frame index.
    m_frameIndex=-1;
    //Update the size.
    updateStreamSize();
    //Update the widget.
    update();
}

void KNBackground::setBackground(const QString &videoPath)
{
    //Get the video directory list.
    QDir videoDir(videoPath);
    if(!videoDir.exists())
    {
        return;
    }
    //Get the directory file content.
    QFileInfoList videoFrameList=videoDir.entryInfoList();
    if(videoFrameList.size()<3)
    {
        //Only have . and ..
        return;
    }
    //Create the video stream.
    QStringList frameNameList;
    frameNameList.reserve(videoFrameList.size()-2);
    for(auto i : videoFrameList)
    {
        if(i.isDir() || i.fileName()=="." || i.fileName()=="..")
        {
            continue;
        }
        //Add the video frame to the stream.
        frameNameList.append(i.absoluteFilePath());
    }
    //Check the stream is empty or not.
    if(frameNameList.isEmpty())
    {
        return;
    }
    //Set the video stream.
    m_videoTimer->stop();
    m_frameNameList=frameNameList;
    //Update the frame index.
    m_frameIndex=0;
    //Update the stream size.
    updateStreamSize();
    m_videoTimer->start();
}

void KNBackground::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Check the current index.
    if(!m_scaledFrame.isNull())
    {
        painter.drawPixmap(0, 0, m_scaledFrame);
    }
}

void KNBackground::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Resize the both widget.
    updateStreamSize();
}

inline void KNBackground::updateStreamSize()
{
    //Update the video stream size.
    m_scaledFrame=m_frame.scaled(width(), height(),
                                 Qt::KeepAspectRatioByExpanding,
                                 Qt::SmoothTransformation);
}
