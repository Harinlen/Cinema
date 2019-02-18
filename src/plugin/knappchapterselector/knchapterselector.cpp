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
#include <QTimer>

#include "knaudiomanager.h"
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
#define BlockSpacing    10
#define BorderWidth     6
#define TextMargin      10
#define IconSize        64

KNChapterSelector::KNChapterSelector(QWidget *parent) :
    KNScrollArea(parent),
    m_chapterModel(new KNChapterModel(this)),
    m_proxyChapterModel(new KNProxyChapterModel(this)),
    m_chapterSearcher(new KNChapterSearcher()),
    m_loadingTimer(new QTimer(this)),
    m_scrollAnime(new QTimeLine(200, this)),
    m_hideAnimation(new QTimeLine(200, this)),
    m_maxColumnCount(1),
    m_itemWidth(knDpi->width(BlockWidth)),
    m_itemHeight(knDpi->height(BlockHeight)),
    m_itemSpacing(knDpi->height(BlockSpacing)),
    m_borderWidth(knDpi->width(BorderWidth)),
    m_startX(0),
    m_currentIndex(-1),
    m_currentState(StateNoDirectory),
    m_loadingIndex(0),
    m_loadingWidth(0),
    m_textMargin(knDpi->width(TextMargin)),
    m_repeat(false)
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
    //Configure the font.
    m_hintTextFont=font();
    m_hintTextFont.setFamily("SST Light");
    m_hintTextFont.setPixelSize(knDpi->height(31));
    m_loadingFont=m_hintTextFont;
    m_loadingFont.setPixelSize(knDpi->height(31));
    m_labelFont=m_hintTextFont;
    m_labelFont.setFamily("SST Medium");
    m_labelFont.setBold(true);
    m_labelFont.setPixelSize(knDpi->height(24));
    setFont(m_labelFont);
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
            this, &KNChapterSelector::onSearchComplete,
            Qt::QueuedConnection);
    connect(m_chapterSearcher, &KNChapterSearcher::searchCancelled,
            this, &KNChapterSelector::startLoadDirectory);
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
    //Configure the loading timer.
    m_loadingTimer->setInterval(32);
    connect(m_loadingTimer, &QTimer::timeout, [=]
    {
        //Update the current index.
        ++m_loadingIndex;
        if(m_loadingIndex>=LoadingFrameCount)
        {
            m_loadingIndex=0;
        }
        //Update the widget.
        update();
    });
    //Configure the label gradient.
    m_titleGradient.setStart(QPoint(0, 0));
    m_titleGradient.setFinalStop(QPoint(0, knDpi->height(BlockHeight)));
    m_titleGradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    m_titleGradient.setColorAt(0.80, QColor(0, 0, 0, 0));
    m_titleGradient.setColorAt(1.0, QColor(0, 0, 0, 180));
    //Load the loading frames.
    char frameIndex[3];
    frameIndex[2]='\0';
    for(int i=0; i<LoadingFrameCount; ++i)
    {
        //Construct the frame data.
        sprintf(frameIndex, "%02d", i+1);
        //Load the pixmap.
        m_loadingRing[i]=QPixmap(QString(":/loading/%1.png").arg(
                                     QString(frameIndex))).scaled(
                    knDpi->size(IconSize, IconSize),
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
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
    return (categoryRow>-1 && categoryRow<m_proxyChapterModel->rowCount()) ?
                categoryRow : -1;
}

void KNChapterSelector::scrollTo(int index, ScrollHint hint)
{
    //Check the index and the validation of max column count.
    if(index<0 || index>=m_proxyChapterModel->rowCount() ||
            m_maxColumnCount==0 ||
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
    //Check the current state, if the state is no directory, directly move to
    //loading state.
    if(m_currentState==StateNoDirectory)
    {
        //Start searching.
        startLoadDirectory();
    }
    else
    {
        //Start the hide animation.
        startHideAnimation();
    }
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
    if(index<-1 || index>=m_proxyChapterModel->rowCount() ||
            m_currentIndex==index)
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
        painter.setFont(m_hintTextFont);
        painter.drawText(rect(), Qt::AlignCenter, m_addDirHint);
        break;
    case StateLoading:
    {
        painter.setFont(m_loadingFont);
        int contentSpacing=knDpi->width(30),
            contentX=(width()-m_loadingRing[0].width()-
                contentSpacing-m_loadingWidth)>>1;
        //Draw the loading ring.
        painter.drawPixmap(contentX,
                           (height()-m_loadingRing[m_loadingIndex].height())>>1,
                           m_loadingRing[m_loadingIndex]);
        //Draw the text.
        painter.drawText(contentX+m_loadingRing[m_loadingIndex].width()+
                         contentSpacing, 0, width(), height(),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         m_loadingHint);
        break;
    }
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
        while(index<m_proxyChapterModel->rowCount())
        {
            QRect itemRect(itemX, itemY, m_itemWidth, m_itemHeight);
            const ChapterUtil::ChapterData &chapterData=
                    m_proxyChapterModel->chapter(index);
            //Check the item cover image.
            if(chapterData.cover.isNull())
            {
                //Null image detected, draw the default image instead.
                painter.fillRect(itemRect,
                                 QColor(255, 255, 255, 108));
            }
            else
            {
                //Draw the item cover image
                painter.drawPixmap(itemX, itemY, chapterData.cover);
            }
            //Draw the title.
            QColor penColor(255, 255, 255);
            painter.save();
            painter.translate(itemRect.topLeft());
            painter.fillRect(QRect(0, 0, itemRect.width(), itemRect.height()),
                             m_titleGradient);
            painter.restore();
            painter.setPen(penColor);
            int textWidth=itemRect.width()-m_textMargin;
            painter.drawText(itemRect.x()+(m_textMargin>>1),
                             itemRect.y()+(m_textMargin>>1),
                             textWidth,
                             itemRect.height()-m_textMargin,
                             Qt::AlignLeft | Qt::AlignBottom,
                             fontMetrics().elidedText(chapterData.title,
                                                      Qt::ElideRight,
                                                      textWidth));
            //Draw the border around the selected item.
            if(index==m_currentIndex)
            {
                QRect borderRect(itemRect.x()-(m_borderWidth>>1),
                                 itemRect.y()-(m_borderWidth>>1),
                                 itemRect.width()+m_borderWidth,
                                 itemRect.height()+m_borderWidth);
                painter.fillRect(borderRect.x(), borderRect.y(),
                                 borderRect.width(), m_borderWidth, penColor);
                painter.fillRect(borderRect.x(), borderRect.y(), m_borderWidth,
                                 borderRect.height(), penColor);
                painter.fillRect(borderRect.x()+borderRect.width()
                                 -m_borderWidth,
                                 borderRect.y(), m_borderWidth,
                                 borderRect.height(),
                                 penColor);
                painter.fillRect(borderRect.x(),
                                 borderRect.y()+borderRect.height()
                                 -m_borderWidth,
                                 borderRect.width(), m_borderWidth,
                                 penColor);
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
    updateParameters(m_proxyChapterModel->rowCount());
}

void KNChapterSelector::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        //Start repeat filter.
        m_repeat=!m_repeat;
        if(m_repeat)
        {
            event->ignore();
            return;
        }
    }
    else
    {
        //Reset the repeat flag.
        m_repeat=false;
    }
    //Check the current state.
    if(m_currentState==StateSelect)
    {
        //Only allowed operate for select mode.
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
        case Qt::Key_Return:
            //Select the current item.
            selectCurrentItem();
            event->accept();
            break;
        default:
            event->ignore();
            break;
        }
    }
    else
    {
        //Not allowed to operate in anyother modes.
        event->ignore();
    }
}

void KNChapterSelector::mouseReleaseEvent(QMouseEvent *event)
{
    //Check the current state.
    if(m_currentState==StateSelect)
    {
        //Check the event position.
        int clickedIndex=indexAt(event->pos());
        if(clickedIndex!=-1)
        {
            //Select the item.
            setCurrentIndex(clickedIndex);
            selectCurrentItem();
        }
        //Accept all the release event.
        event->accept();
    }
}

void KNChapterSelector::retranslate()
{
    m_addDirHint=tr("Please add one directory in the menu.");
    m_loadingHint=tr("Please wait...");
    //Update loading width.
    m_loadingWidth=QFontMetrics(m_loadingFont).width(m_loadingHint);
}

void KNChapterSelector::onRowCountChange(int itemCount)
{
    //Calculate the row count.
    updateParameters(itemCount);
}

void KNChapterSelector::onSearchComplete(
        const QList<ChapterUtil::ChapterData> &chapterList)
{
    //Give the chapter list to chapter model.
    m_chapterModel->setChapterList(chapterList);
    //Update the row count.
    onRowCountChange(chapterList.size());
    //The widget would be changed into showing animation.
    startShowAnimation();
}

void KNChapterSelector::startLoadDirectory()
{
    //Check the search worker state.
    if(m_chapterSearcher->cancel())
    {
        //Wait until the searcher has been cancelled.
        return;
    }
    //Check current directory existance.
    if(m_currentDir.exists())
    {
        //Set the current mode to loading state.
        startLoadingAnimation();
        //Emit the loading signal.
        emit requireStartSearch(m_currentDir.absolutePath());
    }
    else
    {
        //Direct show the error information.
        m_currentState=StateSelect;
        //Update the widget.
        update();
    }
}

void KNChapterSelector::validAndMoveToCurrent()
{
    //Check index range.
    if(m_currentIndex<0)
    {
        m_currentIndex=0;
    }
    else if(m_currentIndex>=m_proxyChapterModel->rowCount())
    {
        m_currentIndex=m_proxyChapterModel->rowCount()-1;
    }
    //Play the audio.
    knAudio->play(KNAudioManager::AudioMove);
    //Update the viewport.
    viewport()->update();
    //Move to the current index.
    scrollTo(m_currentIndex);
}

void KNChapterSelector::stopAllAnimations()
{
    //Stop the loading timer.
    m_loadingTimer->stop();
}

void KNChapterSelector::startHideAnimation()
{
    ;
}

void KNChapterSelector::startLoadingAnimation()
{
    stopAllAnimations();
    //Set current state to loading.
    m_currentState=StateLoading;
    //Start the timer.
    m_loadingIndex=0;
    m_loadingTimer->start();
}

void KNChapterSelector::startShowAnimation()
{
    //Start drawing the shate showing.
    //!FIXME: debugging.
    m_currentState=StateSelect;
    //Update the widget.
    update();
}

inline void KNChapterSelector::selectCurrentItem()
{
    //Emit the signal for select the item.
    emit itemSelected(m_currentIndex);
    //Play the select sound.
    knAudio->play(KNAudioManager::AudioOk);
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
    if(index<0 || index>m_proxyChapterModel->rowCount() || m_maxColumnCount==0)
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
