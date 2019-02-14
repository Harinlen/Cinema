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
#include <QTimeLine>

#include "kndpimanager.h"
#include "knchaptersearcher.h"
#include "knchaptermodel.h"
#include "knproxychaptermodel.h"

#include "knappchapterselector.h"

#define BlockWidth      220
#define BlockHeight     356
#define BlockSpacing    5

KNAppChapterSelector::KNAppChapterSelector(QWidget *parent) :
    QScrollArea(parent),
    m_chapterModel(new KNChapterModel(this)),
    m_proxyChapterModel(new KNProxyChapterModel(this)),
    m_chapterSearcher(new KNChapterSearcher()),
    m_hideAnimation(new QTimeLine(0, this)),
    m_currentState(StateSelect)
{
    //Set chapter model cover size.
    m_proxyChapterModel->setSourceModel(m_chapterModel);

    //Configure the chapter searcher.
    m_chapterSearcher->setCoverSize(knDpi->size(BlockWidth, BlockHeight));
    //Connect the request signal.
    connect(this, &KNAppChapterSelector::requireStartSearch,
            m_chapterSearcher, &KNChapterSearcher::search,
            Qt::QueuedConnection);
    connect(this, &KNAppChapterSelector::requireCancelSearch,
            m_chapterSearcher, &KNChapterSearcher::cancel,
            Qt::QueuedConnection);
    m_chapterSearcher->moveToThread(&m_chapterSearchThread);

    //Start the working thread.
    m_chapterSearchThread.start();
}

KNAppChapterSelector::~KNAppChapterSelector()
{
    //Emit the cancel signal.
    emit requireCancelSearch();
    //Wait the thread quit.
    m_chapterSearchThread.quit();
    m_chapterSearchThread.wait();
    //Clear the searcher.
    m_chapterSearcher->deleteLater();
}

void KNAppChapterSelector::setCurrentDir(const QString &dirPath)
{
    //Save the new directory path.
    m_currentDir.setPath(dirPath);
    //Start the hide animation.
    ;
}

void KNAppChapterSelector::reset()
{
    ;
}

inline void KNAppChapterSelector::startHideAnimation()
{
    ;
}
