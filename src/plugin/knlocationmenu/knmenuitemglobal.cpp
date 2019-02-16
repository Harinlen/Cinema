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
#include "kndpimanager.h"

#include "knmenuitemglobal.h"

KNMenuItemGlobal *KNMenuItemGlobal::ins=nullptr;

KNMenuItemGlobal *KNMenuItemGlobal::instance()
{
    return ins;
}

void KNMenuItemGlobal::initial(QObject *parent)
{
    if(ins)
    {
        return;
    }
    ins=new KNMenuItemGlobal(parent);
}

KNMenuItemGlobal::KNMenuItemGlobal(QObject *parent) : QObject(parent),
    m_tick(QPixmap(":/tick.png"))
{
    //Rescaled the tick item.
    m_tick=m_tick.scaled(knDpi->size(24, 24),
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
}

QPixmap KNMenuItemGlobal::tickIcon() const
{
    return m_tick;
}
