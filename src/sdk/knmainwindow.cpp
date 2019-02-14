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
#include <QLabel>
#include <QKeyEvent>
#include <QFileInfo>

#include "knconfigure.h"
#include "knglobal.h"
#include "kndpimanager.h"
#include "knbootlayer.h"
#include "knapplicationlayer.h"
#include "knmenulayer.h"
#include "knbackground.h"

#include "knmainwindow.h"

#include <QDebug>

#define KeyBackground       "Background"
#define KeyBackgroundVideo  "BackgroundVideo"

KNMainWindow::KNMainWindow(QWidget *parent) : QMainWindow(parent),
    m_background(nullptr),
    m_applicationLayer(nullptr),
    m_menuLayer(nullptr),
    m_bootLayer(nullptr)
{
    setObjectName("MainWindow");
    setFocusPolicy(Qt::StrongFocus);
    //Set the DPI of the main window to the DPI manager.
    knDpi->setDpi(logicalDpiX(), logicalDpiY());
    //Set properties.
    setWindowState(Qt::WindowFullScreen);
    raise();
    //Load the layer.
    m_background=new KNBackground(this);
    m_applicationLayer=new KNApplicationLayer(this);
    m_menuLayer=new KNMenuLayer(this);
    m_bootLayer=new KNBootLayer(this);
    //Configure the boot layer.
    connect(m_bootLayer, &KNBootLayer::requireStartup,
            m_applicationLayer, &KNApplicationLayer::showAnimation);
    //Configure the applicaiton layer.
    m_applicationLayer->setFocusProxy(this);
    //Get the configure.
    m_configure=knGlobal->userConfigure()->getConfigure(objectName());
    //Restore the background.
    if(m_configure->data(KeyBackgroundVideo, false).toBool())
    {
        //Is a video, set the background as video.
        m_background->setBackground(
                    m_configure->data(KeyBackground).toString());
    }
    else
    {
        //Is an image, set the background as image.
        m_background->setBackground(
                    QPixmap(m_configure->data(KeyBackground).toString()));
    }

    //Configure the boot layer.
    m_bootLayer->setFocusProxy(this);
}

void KNMainWindow::appendAppWidget(QWidget *widget)
{
    //Add widget to the container.
    m_applicationLayer->appendAppWidget(widget);
}

void KNMainWindow::resizeEvent(QResizeEvent *event)
{
    //Update the main window.
    QMainWindow::resizeEvent(event);
    //Update the label size.
    if(m_background)
    {
        m_background->resize(width(), height());
        m_applicationLayer->resize(width(), height());
        m_menuLayer->resize(width(), height());
        m_bootLayer->resize(width(), height());
    }
}

void KNMainWindow::showEvent(QShowEvent *event)
{
    //Do the show event.
    QMainWindow::showEvent(event);
    //Start the boot screen event.
    if(m_bootLayer->isVisible())
    {
        // Start the boot animation.
        m_bootLayer->startAnimation();
    }
}

void KNMainWindow::closeEvent(QCloseEvent *event)
{
    //Save the configuration.
    emit requireSaveConfigure();
    //Do the mainwindow close event.
    QMainWindow::closeEvent(event);
}

void KNMainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_QuoteLeft:
        //Check the menu layer state.
        if(!m_menuLayer->isVisible())
        {
            //Show the menu.
            m_menuLayer->setGeometry(rect());
            m_menuLayer->showMenu();
            m_menuLayer->show();
            event->accept();
        }
        break;
    default:
        event->ignore();
        break;
    }
}
