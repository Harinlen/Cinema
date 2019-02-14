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
#include <QKeyEvent>
#include <QTimeLine>

#include "knaudiomanager.h"

#include "knmenulayer.h"

#define Darkness    157
#define Duration    500

KNMenuLayer::KNMenuLayer(QWidget *parent) : QWidget(parent),
    m_timeLine(new QTimeLine(Duration, this)),
    m_darkness(0)
{
    setFocusPolicy(Qt::StrongFocus);
    //Configure the timeline.
    m_timeLine->setUpdateInterval(16);
    m_timeLine->setEasingCurve(QEasingCurve::OutQuad);
    connect(m_timeLine, &QTimeLine::frameChanged, [=](int darkness)
    {
        //Save the darkness.
        m_darkness=darkness;
        //Update the widget.
        update();
    });
    //Hide the widget at beginning.
    hide();
}

void KNMenuLayer::showMenu()
{
    if(m_timeLine->state()==QTimeLine::Running)
    {
        return;
    }
    //Configure the time line.
    m_timeLine->setFrameRange(m_darkness, Darkness);
    m_timeLine->start();
    //Play the audio hint.
    knAudio->play(KNAudioManager::AudioMenuOpen);
    //Change the focus.
    setFocus();
}

void KNMenuLayer::hideMenu()
{
    if(m_timeLine->state()==QTimeLine::Running)
    {
        return;
    }
    //Configure the time line.
    m_timeLine->setFrameRange(m_darkness, 0);
    connect(m_timeLine, &QTimeLine::finished, [=]
    {
        //Disconnect the finished signal.
        disconnect(m_timeLine, &QTimeLine::finished, nullptr, nullptr);
        //Hide the widget.
        hide();
    });
    //Play the audio hint.
    knAudio->play(KNAudioManager::AudioMenuClose);
    //Start the animation.
    m_timeLine->start();
}

void KNMenuLayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);
    //Render the backgorund.
    painter.fillRect(rect(), QColor(0, 0, 0, m_darkness));
}

void KNMenuLayer::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        //Hide the menu.
        hideMenu();
        break;
    default:
        break;
    }
    event->accept();
}
