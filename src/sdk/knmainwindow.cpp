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

#include "knmainwindow.h"

#include <QDebug>

#define KeyBackground "Background"

KNMainWindow::KNMainWindow(QWidget *parent) : QMainWindow(parent),
    m_background(new QLabel(this)),
    m_bootLayer(new KNBootLayer(this))
{
    setObjectName("MainWindow");
    //Set the DPI of the main window to the DPI manager.
    knDpi->setDpi(logicalDpiX(), logicalDpiY());
    //Get the configure.
    m_configure=knGlobal->userConfigure()->getConfigure(objectName());
    // Set properties.
    setWindowState(Qt::WindowFullScreen);
    //Configure the background image.
    m_background->setFocusProxy(this);
    m_background->setScaledContents(true);
    //Load the default background.
    loadDefaultBackground();
    //Load the configure.
    setBackground(m_configure->data(KeyBackground, "").toString());

    //Configure the boot layer.
    m_bootLayer->setFocusProxy(this);
}

void KNMainWindow::setBackground(const QString &filePath)
{
    // - Assume that there is always a background is valid before set.
    //Check the file exist.
    if(QFileInfo(filePath).exists())
    {
        //Load the background as pixmap.
        QPixmap backgroundImage(filePath);
        if(filePath.isNull())
        {
            //Invalid image, do not save the settings.
            return;
        }
        //Set the background.
        m_background->setPixmap(backgroundImage);
        //Save the configure.
        m_configure->setData(KeyBackground, filePath);
    }
}

void KNMainWindow::resizeEvent(QResizeEvent *event)
{
    //Update the main window.
    QMainWindow::resizeEvent(event);
    //Update the label size.
    m_background->resize(width(), height());
    m_bootLayer->resize(width(), height());
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

//    event->accept();
}

inline void KNMainWindow::loadDefaultBackground()
{
    // The default image background is the one which contains in resource file.
    m_background->setPixmap(QPixmap(":/background.png"));
}
