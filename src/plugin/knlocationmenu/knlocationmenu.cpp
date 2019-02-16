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
#include <QBoxLayout>
#include <QLinearGradient>

#include "knconfigure.h"
#include "knmenuitem.h"
#include "knglobal.h"
#include "kndpimanager.h"

#include "knlocationmenu.h"

KNLocationMenu::KNLocationMenu(QWidget *parent) : KNMenuBase(parent),
    m_configure(knGlobal->userConfigure()->getConfigure("Locations")),
    m_opacity(0.0)
{
    //Configure the gradient initial point.
    m_backgroundGradient.setStart(QPoint(0, 0));
    //Configure the gradient color.
    m_backgroundGradient.setColorAt(0.0, QColor(0, 0, 0, 230));
    m_backgroundGradient.setColorAt(1.0, QColor(0, 0, 0, 178));

    //Debug layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(knDpi->margins(14, 14, 119, 14));
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    mainLayout->addStretch();
    mainLayout->addWidget(new KNMenuItem("Automatic", this));
    mainLayout->addWidget(new KNMenuItem("480p", this));
    mainLayout->addWidget(new KNMenuItem("720p", this));
    mainLayout->addWidget(new KNMenuItem("1080i", this));
    mainLayout->addWidget(new KNMenuItem("1080p", this));
    mainLayout->addStretch();
}

void KNLocationMenu::setOpacity(qreal opacity)
{
    //Save the opacity.
    m_opacity=opacity;
    //Update the widget.
    update();
}

void KNLocationMenu::resizeEvent(QResizeEvent *event)
{
    //Resize the menu.
    QWidget::resizeEvent(event);
    //Update the background gradient position.
    m_backgroundGradient.setFinalStop(width(), 0);
}

void KNLocationMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    painter.setOpacity(m_opacity);
    painter.fillRect(rect(), m_backgroundGradient);
    int posX;
    posX=knDpi->width(3);
    painter.setPen(QColor(0x47, 0x4e, 0x53));
    painter.drawLine(posX, 0, posX, height());
    posX=knDpi->width(4);
    painter.setPen(QColor(0x7e, 0x80, 0x82));
    painter.drawLine(posX, 0, posX, height());
}
