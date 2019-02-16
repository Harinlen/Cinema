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

#include "knmenubase.h"
#include "kndpimanager.h"
#include "knaudiomanager.h"

#include "knmenulayer.h"

#define Darkness    157
#define Duration    333
#define MenuWidth   480

KNMenuLayer::KNMenuLayer(QWidget *parent) : QWidget(parent),
    m_timeLine(new QTimeLine(Duration, this)),
    m_menuWidget(nullptr),
    m_showing(false)
{
    setFocusPolicy(Qt::StrongFocus);
    //Configure the timeline.
    m_timeLine->setUpdateInterval(16);
    m_timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_timeLine, &QTimeLine::frameChanged, [=](int offset)
    {
        //Save the darkness.
        m_menuWidget->move(width()-offset, 0);
        //Set the menu status.
        m_menuWidget->setOpacity(m_showing?m_timeLine->currentValue():
                                           1.0-m_timeLine->currentValue());
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
    m_showing=true;
    //Prepare the hovering.
    m_menuWidget->prepareHover();
    //Configure the animation.
    m_timeLine->setFrameRange(0, m_menuWidget->width());
    //Start animation.
    m_menuWidget->move(width(), 0);
    m_timeLine->start();
    //Play the audio hint.
    knAudio->play(KNAudioManager::AudioMenuOpen);
    //Change the focus.
    m_menuWidget->setFocus();
}

void KNMenuLayer::hideMenu()
{
    if(m_timeLine->state()==QTimeLine::Running)
    {
        return;
    }
    m_showing=false;
    //Configure the time line.
    m_timeLine->setFrameRange(m_menuWidget->width(), 0);
    connect(m_timeLine, &QTimeLine::finished, [=]
    {
        //Disconnect the finished signal.
        disconnect(m_timeLine, &QTimeLine::finished, nullptr, nullptr);
        //Hide the widget.
        hide();
    });
    //Start the animation.
    m_timeLine->start();
    //Play the audio hint.
    knAudio->play(KNAudioManager::AudioMenuClose);
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

void KNMenuLayer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Check the menu widget.
    if(m_menuWidget)
    {
        //Update the menu widget size.
        m_menuWidget->resize(knDpi->width(MenuWidth), height());
    }
}

QWidget *KNMenuLayer::menuWidget() const
{
    return m_menuWidget;
}

void KNMenuLayer::setMenuWidget(KNMenuBase *menuWidget)
{
    //Save the menu widget pointer.
    m_menuWidget = menuWidget;
    //Link the request signal from menu widget.
    connect(m_menuWidget, &KNMenuBase::requireHideMenu,
            this, &KNMenuLayer::hideMenu);
    //Transfer the relationship.
    m_menuWidget->setParent(this);
    m_menuWidget->move(width(), 0);
    m_menuWidget->resize(knDpi->width(MenuWidth), height());
    //Transfer the focus proxy.
    setFocusProxy(m_menuWidget);
}
