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
#include "knchapterselector.h"
#include "kndpimanager.h"

#include "knappchapterselector.h"

#include <QDebug>

#define ChapterSelectorY    57

KNAppChapterSelector::KNAppChapterSelector(QWidget *parent) :
    KNAppChapterSelectorBase(parent),
    m_chapterSelector(new KNChapterSelector(this))
{
    //Set properties.
    setFocusProxy(m_chapterSelector);
    //Configure the chapter selector.
    m_chapterSelector->move(knDpi->pos(0, ChapterSelectorY));
}

void KNAppChapterSelector::onLibraryPathChanged(const QString &libraryPath)
{
    //Change the library path of the chapter selector.
    m_chapterSelector->setCurrentDir(libraryPath);
}

void KNAppChapterSelector::resizeEvent(QResizeEvent *event)
{
    //Resize the widgetr size.
    QWidget::resizeEvent(event);
    //Resize the chapter selector.
    m_chapterSelector->resize(width(),
                              height()-knDpi->height(ChapterSelectorY));
}
