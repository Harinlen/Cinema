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
#include <QBoxLayout>
#include <QLabel>

#include "kndpimanager.h"
#include "knclocklabel.h"

#include "knmainapplicationheader.h"

KNMainApplicationHeader::KNMainApplicationHeader(QWidget *parent) :
    QWidget(parent),
    m_clock(new KNClockLabel(this))
{
    setContentsMargins(knDpi->margins(102, 0, 100, 25));
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255, 255, 255, 200));
    setPalette(pal);
    //Configure the font.
    QFont font=this->font();
    font.setFamily("SST Light");
    font.setPixelSize(knDpi->height(34));
    setFont(font);
    //Configure the clock widget.
    m_clock->setFont(font);

    //Configure the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignBottom);
    //Add widgets to main layout.
    mainLayout->addWidget(new QLabel("Library", this), 1);
    mainLayout->addWidget(m_clock);
}
