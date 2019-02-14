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
#include <QScrollBar>
#include <QPainter>

#include "knchapterutil.h"
#include "kndpimanager.h"
#include "knchaptersearcher.h"
#include "knchaptermodel.h"
#include "knproxychaptermodel.h"
#include "knlocalemanager.h"
#include "knthememanager.h"

#include "knappchapterselector.h"

#define BlockWidth      220
#define BlockHeight     356
#define BlockSpacing    5

KNAppChapterSelector::KNAppChapterSelector(QWidget *parent) :
    KNAppChapterSelectorBase(parent),
    m_chapterModel(new KNChapterModel(this)),
    m_proxyChapterModel(new KNProxyChapterModel(this)),
    m_chapterSearcher(new KNChapterSearcher()),
    m_hideAnimation(new QTimeLine(200, this)),
    m_hItemCount(1),
    m_itemWidth(knDpi->width(BlockWidth)),
    m_itemHeight(knDpi->height(BlockHeight)),
    m_itemSpacing(knDpi->height(BlockSpacing)),
    m_currentState(StateShowing)
{
    setObjectName("AppChapterSelector");
    //Register types.
    qRegisterMetaType<QList<ChapterUtil::ChapterData>>(
                "QList<ChapterUtil::ChapterData>");
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    connect(m_chapterSearcher, &KNChapterSearcher::searchComplete,
            m_chapterModel, &KNChapterModel::setChapterList,
            Qt::QueuedConnection);
    connect(m_chapterSearcher, &KNChapterSearcher::searchCancelled,
            [=]
    {
        ;
    });
    //Configure vertical scroll bar.
    verticalScrollBar()->setSingleStep(m_itemHeight>>2);
    verticalScrollBar()->setPageStep(m_itemHeight+m_itemSpacing);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, [=]{update();});
    m_chapterSearcher->moveToThread(&m_chapterSearchThread);
    //Set the theme.
    knTheme->registerWidget(this);
    //Link multiple locale.
    knI18n->link(this, &KNAppChapterSelector::retranslate);
    retranslate();
    //Start the working thread.
    m_chapterSearchThread.start();

    onRowCountChange(186);
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
    startHideAnimation();
}

void KNAppChapterSelector::reset()
{
    //Clear the menu.
    m_chapterModel->reset();
    //Reset the current state.
    startHideAnimation();
}

void KNAppChapterSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Check the current state.
    switch(m_currentState)
    {
    case StateNoDirectory:
        painter.setPen(palette().color(QPalette::WindowText));
        painter.drawText(rect(), Qt::AlignCenter, m_addDirHint);
        break;
    default:
    {
        //Should never arrive here.

        //Get the first row start index.
        int verticalValue=verticalScrollBar()->value(),
            startRow=verticalValue/(m_itemSpacing+m_itemHeight),
            itemY=startRow*(m_itemSpacing+m_itemHeight)-verticalValue
                +m_itemSpacing,
            index=startRow*m_hItemCount,
            itemStartX=m_startX+m_itemSpacing,
            itemX=itemStartX, currentColumn=0;
        while(index<186)
        {
            //Draw the item.
            painter.drawRect(itemX, itemY, m_itemWidth, m_itemHeight);
            //Move to the next item
            ++index;
            //Move to the next column.
            ++currentColumn;
            if(currentColumn==m_hItemCount)
            {
                //Move to next row.
                itemY+=m_itemHeight+m_itemSpacing;
                itemX=itemStartX;
                currentColumn=0;
                //Check the Y position.
                if(itemY>=height())
                {
                    //No need to keep drawing.
                    break;
                }
            }
            else
            {
                //Move to next column.
                itemX+=m_itemWidth+m_itemSpacing;
            }
        }
        break;
    }
    }
}

void KNAppChapterSelector::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KNAppChapterSelectorBase::resizeEvent(event);
    //Update the row item count.
    m_hItemCount=(width()-m_itemSpacing) / (m_itemSpacing+m_itemWidth);
    m_startX=(width()-m_hItemCount*(m_itemSpacing+m_itemWidth)-
              m_itemSpacing)>>1;
    //Update parameters.
//    updateParameters(m_chapterModel->rowCount());
    updateParameters(186);
}

void KNAppChapterSelector::retranslate()
{
    m_addDirHint=tr("Please add one directory in the menu.");
}

void KNAppChapterSelector::onRowCountChange(int itemCount)
{
    //Calculate the row count.
    updateParameters(itemCount);
}

inline void KNAppChapterSelector::startHideAnimation()
{
    ;
}

inline void KNAppChapterSelector::updateParameters(int itemCount)
{
    int rowCount = itemCount / m_hItemCount;
    //Calculate the entire height.
    int entireHeight = rowCount * (m_itemHeight +
                                   m_itemSpacing) + m_itemSpacing;
    //Update the vertical scroll bar.
    verticalScrollBar()->setRange(0, entireHeight - height());
}
