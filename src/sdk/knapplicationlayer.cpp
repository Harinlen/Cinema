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
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "knhwidgetswitcher.h"
#include "knmainapplicationheader.h"
#include "kndpimanager.h"

#include "knapplicationlayer.h"

KNApplicationLayer::KNApplicationLayer(QWidget *parent) : QWidget(parent),
    m_container(new KNHWidgetSwitcher(this)),
    m_header(new KNMainApplicationHeader(this)),
    m_headerAnimation(generateAnimation(m_header)),
    m_containerAnimation(generateAnimation(m_container)),
    m_showAnimation(new QParallelAnimationGroup(this))
{
    //Configure the header.
    m_header->setFixedHeight(knDpi->height(150));
    m_header->setFocusProxy(parentWidget());
    m_header->hide();
    //Configure the container.
    m_container->hide();

    //Add the animation to the group.
    m_showAnimation->addAnimation(m_headerAnimation);
    m_showAnimation->addAnimation(m_containerAnimation);
}

void KNApplicationLayer::showAnimation()
{
    //Set the header animation position.
    setRange(m_headerAnimation,
             QPoint(0, -m_header->height()), QPoint(0, 0));
    setRange(m_containerAnimation,
             QPoint(0, height()), QPoint(0, m_header->height()));
    //Show the animation.
    m_showAnimation->start();
}

void KNApplicationLayer::appendAppWidget(QWidget *widget)
{
    //Add widget to container.
    m_container->addWidget(widget);
}

void KNApplicationLayer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the header.
    m_header->resize(width(), m_header->height());
    m_container->resize(width(), height()-m_header->height());
}

inline void KNApplicationLayer::setRange(QPropertyAnimation *animation,
                                         const QPoint &start, const QPoint &end)
{
    animation->setStartValue(start);
    animation->setEndValue(end);
    //Configure the widget start position.
    QWidget *targetWidget=static_cast<QWidget *>(animation->targetObject());
    targetWidget->move(start);
    targetWidget->show();
}

inline QPropertyAnimation *KNApplicationLayer::generateAnimation(
        QWidget *target)
{
    QPropertyAnimation *animation=new QPropertyAnimation(target, "pos", this);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    return animation;
}
