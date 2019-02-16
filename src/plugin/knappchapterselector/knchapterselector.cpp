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

#include "knaudiomanager.h"
#include "knchapterutil.h"
#include "kndpimanager.h"
#include "knchaptersearcher.h"
#include "knchaptermodel.h"
#include "knproxychaptermodel.h"
#include "knlocalemanager.h"
#include "knthememanager.h"

#include "knchapterselector.h"

#include <QDebug>

#define BlockWidth      220
#define BlockHeight     356
#define BlockSpacing    5

KNChapterSelector::KNChapterSelector(QWidget *parent) :
    KNScrollArea(parent),
    m_chapterModel(new KNChapterModel(this)),
    m_proxyChapterModel(new KNProxyChapterModel(this)),
    m_chapterSearcher(new KNChapterSearcher()),
    m_scrollAnime(new QTimeLine(200, this)),
    m_hideAnimation(new QTimeLine(200, this)),
    m_maxColumnCount(1),
    m_itemWidth(knDpi->width(BlockWidth)),
    m_itemHeight(knDpi->height(BlockHeight)),
    m_itemSpacing(knDpi->height(BlockSpacing)),
    m_startX(0),
    m_currentIndex(-1),
    m_currentState(StateShowing)
{
    setObjectName("AppChapterSelector");
    //Register types.
    qRegisterMetaType<QList<ChapterUtil::ChapterData>>(
                "QList<ChapterUtil::ChapterData>");
    //Set properties.
    setFocusPolicy(Qt::WheelFocus);
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Set chapter model cover size.
    m_proxyChapterModel->setSourceModel(m_chapterModel);
    //Configure the chapter searcher.
    m_chapterSearcher->setCoverSize(knDpi->size(BlockWidth, BlockHeight));
    //Connect the request signal.
    connect(this, &KNChapterSelector::requireStartSearch,
            m_chapterSearcher, &KNChapterSearcher::search,
            Qt::QueuedConnection);
    connect(this, &KNChapterSelector::requireCancelSearch,
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
    //Configure the timeline.
    m_scrollAnime->setUpdateInterval(16);
    m_scrollAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollAnime, &QTimeLine::frameChanged, [=](int scrollValue)
    {
        //Set the value to scroll bar.
        verticalScrollBar()->setValue(scrollValue);
        //Update the viewport.
        viewport()->update();
    });
    //Set the theme.
    knTheme->registerWidget(this);
    //Link multiple locale.
    knI18n->link(this, &KNChapterSelector::retranslate);
    retranslate();
    //Start the working thread.
    m_chapterSearchThread.start();
}

KNChapterSelector::~KNChapterSelector()
{
    //Emit the cancel signal.
    emit requireCancelSearch();
    //Wait the thread quit.
    m_chapterSearchThread.quit();
    m_chapterSearchThread.wait();
    //Clear the searcher.
    m_chapterSearcher->deleteLater();
}

int KNChapterSelector::indexAt(const QPoint &point) const
{
    //Calculate the point content position and the line of the point.
    int spacingWidth=m_itemWidth+m_itemSpacing,
        spacingHeight=m_itemHeight+m_itemSpacing,
        pointContentY=verticalScrollBar()->value()+point.y(),
        itemLine=pointContentY/spacingHeight;
    //Check whether mouse click on a row spacing part.
    if(pointContentY-itemLine*spacingHeight<m_itemSpacing)
    {
        return -1;
    }
    //Calculate the column of the position.
    int itemX=point.x()-m_startX,
        itemColumn=itemX/spacingWidth,
        pointXInItem=itemX-itemColumn*spacingWidth;
    //Check whether mouse click on a column spacing part.
    if(pointXInItem<m_itemSpacing || pointXInItem>m_itemSpacing+m_itemWidth)
    {
        return -1;
    }
    //Calculate the item category index.
    int categoryRow=itemLine*m_maxColumnCount+itemColumn;
    //Check if the category row vaild.
    //We should check the category proxy model, because the point is a display
    //variable.
    return (categoryRow>-1 && categoryRow<186) ? categoryRow : -1;
}

void KNChapterSelector::scrollTo(int index, ScrollHint hint)
{
    //Check the index and the validation of max column count.
    if(index<0 || index>=186 || m_maxColumnCount==0 ||
            //Check whether we need to move the vertical scroll bar
            (hint==EnsureVisible &&
             rect().contains(visualRect(index), true)))
    {
        return;
    }
    //Use timeline to move to the position.
    m_scrollAnime->stop();
    m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                 indexScrollBarValue(index, hint));
    m_scrollAnime->start();
    //Update the painting.
    viewport()->update();
}

QRect KNChapterSelector::visualRect(int index) const
{
    //Get the item content rect.
    QRect indexRect=itemContentRect(index);
    //If the rect is vaild, remove the value of the scroll bar to get the
    //position in the rect.
    return indexRect.isValid()?
                QRect(indexRect.left()-horizontalScrollBar()->value(),
                      indexRect.top()-verticalScrollBar()->value(),
                      indexRect.width(),
                      indexRect.height()):
                QRect();
}

void KNChapterSelector::setCurrentDir(const QString &dirPath)
{
    //Save the new directory path.
    m_currentDir.setPath(dirPath);
    //Start the hide animation.
    startHideAnimation();
}

void KNChapterSelector::reset()
{
    //Clear the menu.
    m_chapterModel->reset();
    //Reset the current state.
    startHideAnimation();
}

void KNChapterSelector::setCurrentIndex(int index)
{
    //Check the index validation.
    if(index<-1 || index>=186 || m_currentIndex==index)
    {
        return;
    }
    //Save the current index.
    m_currentIndex=index;
    //Move to the index.
    validAndMoveToCurrent();
    //Update the view port.
    viewport()->update();
}

void KNChapterSelector::paintEvent(QPaintEvent *event)
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
    case StateSelect:
    case StateHiding:
    case StateShowing:
    {
        //Get the first row start index.
        int verticalValue=verticalScrollBar()->value(),
            startRow=verticalValue/(m_itemSpacing+m_itemHeight),
            itemY=startRow*(m_itemSpacing+m_itemHeight)-verticalValue
            +m_itemSpacing,
            index=startRow*m_maxColumnCount,
            itemStartX=m_startX+m_itemSpacing,
            itemX=itemStartX, currentColumn=0;
        //Clear the painter.
        painter.setPen(Qt::NoPen);
        while(index<186)
        {
            QRect itemRect(itemX, itemY, m_itemWidth, m_itemHeight);

            //Draw the item.
            //!FIXME: replace this part of code with the image.
            painter.fillRect(itemRect,
                             QColor(255, 255, 255, 108));
            //Draw the border around the selected item.
            if(index==m_currentIndex)
            {
                painter.setPen(QColor(255, 255, 255, 200));
                painter.drawRect(itemRect);
                painter.setPen(Qt::NoPen);
            }
            //Move to the next item
            ++index;
            //Move to the next column.
            ++currentColumn;
            if(currentColumn==m_maxColumnCount)
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
    default:
    {
        //Should never arrive here.
        break;
    }
    }
}

void KNChapterSelector::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KNScrollArea::resizeEvent(event);
    //Update the row item count.
    m_maxColumnCount=(width()-m_itemSpacing) / (m_itemSpacing+m_itemWidth);
    m_startX=(width()-m_maxColumnCount*(m_itemSpacing+m_itemWidth)-
              m_itemSpacing)>>1;
    //Update parameters.
//    updateParameters(m_chapterModel->rowCount());
    updateParameters(186);
}

void KNChapterSelector::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W: //Up
        if(m_currentIndex>=m_maxColumnCount)
        {
            m_currentIndex-=m_maxColumnCount;
            validAndMoveToCurrent();
        }
        event->accept();
        break;
    case Qt::Key_S: //Down
        m_currentIndex+=m_maxColumnCount;
        validAndMoveToCurrent();
        event->accept();
        break;
    case Qt::Key_A: //Left
        --m_currentIndex;
        validAndMoveToCurrent();
        event->accept();
        break;
    case Qt::Key_D: //Right
        ++m_currentIndex;
        validAndMoveToCurrent();
        event->accept();
        break;
    case Qt::Key_Enter: //Enter
        event->accept();
        break;
    default:
        event->ignore();
        break;
    }
}

void KNChapterSelector::mouseReleaseEvent(QMouseEvent *event)
{
    //Check the event position.
    int clickedIndex=indexAt(event->pos());
    if(clickedIndex!=-1)
    {
        setCurrentIndex(clickedIndex);
    }
    //Accept all the release event.
    event->accept();
}

void KNChapterSelector::retranslate()
{
    m_addDirHint=tr("Please add one directory in the menu.");
}

void KNChapterSelector::onRowCountChange(int itemCount)
{
    //Calculate the row count.
    updateParameters(itemCount);
}

inline void KNChapterSelector::validAndMoveToCurrent()
{
    //Check index range.
    if(m_currentIndex<0)
    {
        m_currentIndex=0;
    }
    else if(m_currentIndex>=186)
    {
        m_currentIndex=185;
    }
    //Play the audio.
    knAudio->play(KNAudioManager::AudioMove);
    //Update the viewport.
    viewport()->update();
    //Move to the current index.
    scrollTo(m_currentIndex);
}

inline void KNChapterSelector::startHideAnimation()
{
    ;
}

inline void KNChapterSelector::updateParameters(int itemCount)
{
    //Get the row count of the view.
    int rowCount = itemCount / m_maxColumnCount;
    if(rowCount * m_maxColumnCount < itemCount)
    {
        ++rowCount;
    }
    //Calculate the entire height.
    int entireHeight = rowCount * (m_itemHeight +
                                   m_itemSpacing) + m_itemSpacing;

    //Update the vertical scroll bar.
    verticalScrollBar()->setRange(0, entireHeight - height());
    //Check the item count.
    m_currentIndex=(itemCount>0)?0:-1;
}

inline int KNChapterSelector::indexScrollBarValue(int index, ScrollHint hint)
{
    //Get the top of index position, set it to scroll value.
    int spacingHeight=m_itemSpacing+m_itemHeight,
        topPositionValue=index/m_maxColumnCount*spacingHeight+m_itemSpacing;
    //Change the item content Y according to the hint.
    switch(hint)
    {
    case PositionAtTop:
        //No need to change.
        return topPositionValue;
    case PositionAtCenter:
        //Reduce a half of the viewer height to move up.
        return topPositionValue-((height()-spacingHeight)>>1);
    case PositionAtBottom:
        //Reduce the whole viewer height to move up.
        return topPositionValue-height()+spacingHeight;
    default:
        //Now, the index item must be a unvisible one in the viewer.
        //We have already has the top position, calculate the bottom position,
        //and calculate the distence of the current vertical scroll bar's value
        //to these two position.
        int bottomPositionValue=topPositionValue-height()+spacingHeight;
        //If to the bottom is lesser than to top, change the value.
        return (qAbs(verticalScrollBar()->value()-bottomPositionValue)<
                qAbs(verticalScrollBar()->value()-topPositionValue))?
                    bottomPositionValue:
                    topPositionValue;
    }
}

QRect KNChapterSelector::itemContentRect(int index) const
{
    //Check the index first.
    if(index<0 || index>186 || m_maxColumnCount==0)
    {
        return QRect();
    }
    //Calculate the item line.
    int itemLine=index/m_maxColumnCount;
    //Calculate the rect.
    return QRect((index-itemLine*m_maxColumnCount)*(m_itemWidth+m_itemSpacing)+
                 m_startX,
                 itemLine*(m_itemHeight+m_itemSpacing)+m_itemSpacing,
                 m_itemWidth,
                 m_itemHeight);
}
