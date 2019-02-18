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
#include <QFileInfo>
#include <QJsonArray>
#include <QBoxLayout>
#include <QLinearGradient>

#include "knmenuitemglobal.h"
#include "knaudiomanager.h"
#include "knconfigure.h"
#include "knmenuitem.h"
#include "knglobal.h"
#include "kndpimanager.h"
#include "knlocalemanager.h"

#include "knlocationmenu.h"

#include <QDebug>

KNLocationMenu::KNLocationMenu(QWidget *parent) : KNMenuBase(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_addPathItem(nullptr),
    m_configure(knGlobal->userConfigure()->getConfigure("Locations")),
    m_opacity(0.0),
    m_currentHover(0),
    m_currentIndex(-1)
{
    //Initial the global.
    KNMenuItemGlobal::initial(this);
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    //Configure the gradient.
    m_backgroundGradient.setStart(QPoint(0, 0));
    m_backgroundGradient.setColorAt(0.0, QColor(0, 0, 0, 230));
    m_backgroundGradient.setColorAt(1.0, QColor(0, 0, 0, 178));

    //Configure the main layout;
    m_mainLayout->setContentsMargins(knDpi->margins(14, 14, 119, 14));
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    //Add the path added item.
    m_mainLayout->addStretch();
    m_mainLayout->addStretch();

    //Configure the add path item.
    m_addPathItem=new KNMenuItem(this);
    m_addPathItem->setHovering(true);
    addItem(m_addPathItem);
    //Link the translation.
    knI18n->link(this, &KNLocationMenu::retranslate);
    retranslate();
}

void KNLocationMenu::addItem(KNMenuItem *item)
{
    //Add widget to the layout.
    m_mainLayout->insertWidget(m_mainLayout->count()-1, item);
    //Add item to the list.
    m_itemList.append(item);
    //Link the item
    connect(item, &KNMenuItem::hovering,
            this, &KNLocationMenu::onItemHover);
    connect(item, &KNMenuItem::clicked,
            this, &KNLocationMenu::onItemClicked);
}

void KNLocationMenu::setOpacity(qreal opacity)
{
    //Save the opacity.
    m_opacity=opacity;
    //Update the widget.
    update();
}

void KNLocationMenu::loadFromConfigure()
{
    //Get the configure information.
    QJsonArray pathList=
            m_configure->data("LibraryPaths", QJsonArray()).toJsonArray();
    //Clear the path list.
    m_pathList=QStringList();
    m_pathList.reserve(pathList.size());
    for(auto i : pathList)
    {
        //Convert the content.
        m_pathList.append(i.toString());
        //Create menu item for the path.
        KNMenuItem *menuItem=new KNMenuItem(this);
        addItem(menuItem);
    }
    //Update the item text.
    retranslate();
    //Set the current index.
    int savedIndex=m_configure->data("CurrentIndex", -1).toInt();
    if(savedIndex!=-1)
    {
        //For valid data, set the index.
        setCurrentIndex(savedIndex);
    }
}

void KNLocationMenu::prepareHover()
{
    //Check whether we have at least one index.
    if(m_pathList.isEmpty())
    {
        //Set the hover to be 0.
        setCurrentHover(0, false);
    }
    else
    {
        //Move to the first one.
        setCurrentHover(1, false);
    }
}

void KNLocationMenu::setCurrentHover(int index, bool animated)
{
    if(index>-1 && index<m_itemList.size() && m_currentHover!=index)
    {
        //Remove the original hover.
        m_itemList.at(m_currentHover)->setHovering(false, animated);
        //Set the current index.
        m_currentHover=index;
        //Update the widget.
        m_itemList.at(m_currentHover)->setHovering(true, animated);
        //Play the sound.
        knAudio->play(KNAudioManager::AudioMove);
    }
}

void KNLocationMenu::setCurrentIndex(int index)
{
    //When index is invalid or the same, ignore the request.
    if(index<0 || index>=m_pathList.size() || index==m_currentIndex)
    {
        return;
    }
    //Check the original current index.
    if(m_currentIndex>-1 && m_currentIndex<m_pathList.size())
    {
        //Uncheck the item.
        m_itemList.at(m_currentIndex+1)->setSelected(false);
    }
    //Set the current index.
    m_currentIndex=index;
    //Check the current index.
    if(m_currentIndex>-1 && m_currentIndex<m_pathList.size())
    {
        //Uncheck the item.
        m_itemList.at(m_currentIndex+1)->setSelected(true);
        //Save the data to configure
        m_configure->setData("CurrentIndex", m_currentIndex);
        //Emit the item changed signal.
        emit libraryPathChanged(m_pathList.at(m_currentIndex));
    }
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
    //Set the opacity.
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

void KNLocationMenu::keyPressEvent(QKeyEvent *event)
{
    //Detect for only up, down and enter.
    switch(event->key())
    {
    case Qt::Key_W:
        setCurrentHover(m_currentHover-1);
        event->accept();
        break;
    case Qt::Key_S:
        setCurrentHover(m_currentHover+1);
        event->accept();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        //Check the current hover.
        if(m_currentHover==0)
        {
            //!FIXME: add code here.
        }
        else
        {
            //Set the hover item as the selected item.
            setCurrentIndex(m_currentHover-1);
            //Play the okay audio
            knAudio->play(KNAudioManager::AudioOk);
        }
        //Hide the menu.
        emit requireHideMenu();
        break;
    default:
        event->ignore();
        break;
    }
}

void KNLocationMenu::retranslate()
{
    m_addPathItem->setText(tr("Add library path"));
    //Update the path item list.
    for(int i=0; i<m_pathList.size(); ++i)
    {
        //Get the path, check the folder name.
        QFileInfo targetPath(m_pathList.at(i));
        KNMenuItem *menuItem=m_itemList.at(i+1);
        //Check the path is drive or not.
        if(targetPath.isDir() && targetPath.fileName().isEmpty())
        {
            //It should be a drive.
            menuItem->setText(tr("Drive (%1)").arg(
                                  targetPath.absolutePath().left(2)));
        }
        else
        {
            //Display the name of the folder.
            menuItem->setText(targetPath.fileName());
        }
    }
}

void KNLocationMenu::onItemHover()
{
    //Cast the sender as item.
    KNMenuItem *menuItem=qobject_cast<KNMenuItem *>(sender());
    //Find the item in the list.
    int itemIndex=m_itemList.indexOf(menuItem);
    if(itemIndex!=-1)
    {
        //We find the item.
        //Unhover the current item.
        m_itemList.at(m_currentHover)->setHovering(false);
        //Save the new item.
        m_currentHover=itemIndex;
        //Hover the new item.
        m_itemList.at(m_currentHover)->setHovering(true);
    }
}

void KNLocationMenu::onItemClicked()
{
    //Cast the sender as item.
    KNMenuItem *menuItem=qobject_cast<KNMenuItem *>(sender());
    //Find the item in the list.
    int itemIndex=m_itemList.indexOf(menuItem);
    if(itemIndex!=-1)
    {
        //We find the item.
        setCurrentIndex(itemIndex-1);
    }
}
