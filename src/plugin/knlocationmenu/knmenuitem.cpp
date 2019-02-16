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
#include <QTimeLine>

#include "kndpimanager.h"

#include "knmenuitem.h"

#define FadeOutDuration 433
#define FadeInDuration  167
#define TextX           57

KNMenuItem::KNMenuItem(const QString &text, QWidget *parent) : QWidget(parent),
    m_fadeAnimation(new QTimeLine(200, this)),
    m_opacity(0.0),
    m_borderWidth(1)
{
    //Set properties.
    setMinimumWidth(knDpi->width(347));
    setFixedHeight(knDpi->height(75));
    setText(text);
    //Configure the font.
    QFont itemFont=font();
    itemFont.setFamily("SST Light");
    itemFont.setPixelSize(31);
    setFont(itemFont);
    //Configure the animation.
    m_fadeAnimation->setUpdateInterval(16);
    connect(m_fadeAnimation, &QTimeLine::frameChanged, [=](int frame)
    {
        //Shrink the width.
        m_borderWidth = frame / 63;
        if(m_borderWidth<1)
        {
            m_borderWidth=1;
        }
        //Change the opacity.
        m_opacity=static_cast<qreal>(frame) / 255.0;
        //Update the widget.
        update();
    });
}

void KNMenuItem::enterEvent(QEvent *event)
{
    //Enter the widget.
    QWidget::enterEvent(event);
    //Start fade in.
    startAnimeFadeIn();
}

void KNMenuItem::leaveEvent(QEvent *event)
{
    //Leave the widget.
    QWidget::leaveEvent(event);
    //Start fade out.
    startAnimeFadeOut();
}

void KNMenuItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the content.
    //!FIXME: add code here.;
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(QRect(TextX, 0, width(), height()),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_text);
    //Draw the border.
    QPen border;
    border.setColor(QColor(255, 255, 255));
    border.setWidth(m_borderWidth);
    painter.setOpacity(m_opacity);
    painter.setPen(border);
    painter.drawRect(rect());
}

void KNMenuItem::setHovering(bool isHover)
{
    if(m_isHover!=isHover)
    {
        //Save the hover state.
        m_isHover=isHover;
        //Must be different.
        if(m_isHover)
        {
            //Start fade in.
            startAnimeFadeIn();
        }
        else
        {
            //Start fade out.
            startAnimeFadeOut();
        }
    }
    //Repaint the widget.
    update();
}

inline void KNMenuItem::startAnimeFadeIn()
{
    //Force stop the previous status.
    m_fadeAnimation->stop();
    //Configure the time line.
    m_fadeAnimation->setDuration(FadeInDuration);
    m_fadeAnimation->setFrameRange(static_cast<int>(m_opacity*255.0), 255);
    //Start the fade animation.
    m_fadeAnimation->start();
    //Emit hovering state.
    emit hovering();
}

inline void KNMenuItem::startAnimeFadeOut()
{
    //Force stop the previous status.
    m_fadeAnimation->stop();
    //Configure the time line.
    m_fadeAnimation->setDuration(FadeOutDuration);
    m_fadeAnimation->setFrameRange(static_cast<int>(m_opacity*255.0), 0);
    //Start the fade animation.
    m_fadeAnimation->start();
    //Update the hovering state.
    m_isHover=false;
}

QString KNMenuItem::text() const
{
    return m_text;
}

void KNMenuItem::setText(const QString &text)
{
    //Set the text.
    m_text = text;
    //Update the widget.
    update();
}
