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
#include "knmenuitemglobal.h"

#include "knmenuitem.h"

#define FadeOutDuration 433
#define FadeInDuration  167
#define TextX           57

KNMenuItem::KNMenuItem(QWidget *parent, const QString &text) :
    QAbstractButton(parent),
    m_fadeAnimation(new QTimeLine(200, this)),
    m_opacity(0.0),
    m_borderWidth(1),
    m_isSelected(false),
    m_isHover(false)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setMinimumWidth(knDpi->width(347));
    setFixedHeight(knDpi->height(75));
    setText(text);
    //Configure the font.
    QFont itemFont=font();
    itemFont.setFamily("SST Light");
    itemFont.setPixelSize(knDpi->height(31));
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
    QAbstractButton::enterEvent(event);
    //Emit the signal.
    emit hovering();
}

void KNMenuItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Check whether the menu item is checked.
    if(m_isSelected)
    {
        //Draw the selected tick.
        const QPixmap &tickIcon=KNMenuItemGlobal::instance()->tickIcon();
        painter.drawPixmap(knDpi->width(14), (height()-tickIcon.height())>>1,
                           tickIcon);
    }
    //Draw the content.
    painter.setPen(QColor(255, 255, 255));
    int textX = knDpi->width(TextX), textWidth=width()-textX;
    painter.drawText(QRect(textX, 0, textWidth, height()),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     fontMetrics().elidedText(text(), Qt::ElideRight,
                                              textWidth));
    //Draw the border.
    QPen border;
    border.setColor(QColor(255, 255, 255));
    border.setWidth(m_borderWidth);
    painter.setOpacity(m_opacity);
    painter.setPen(border);
    painter.drawRect(rect());
}

void KNMenuItem::setHovering(bool isHover, bool animated)
{
    if(m_isHover!=isHover)
    {
        //Save the hover state.
        m_isHover=isHover;
        //Must be different, check the animation settings.
        if(animated)
        {
            //Play the specific animation.
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
        else
        {
            //For non-animated, we have to change the opacity.
            m_opacity = m_isHover ? 1.0 : 0.0;
            m_borderWidth = m_isHover ? 4 : 1;
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

bool KNMenuItem::isSelected() const
{
    return m_isSelected;
}

void KNMenuItem::setSelected(bool isSelected)
{
    //Save the selected state.
    m_isSelected = isSelected;
    //Update the item widget.
    update();
}
