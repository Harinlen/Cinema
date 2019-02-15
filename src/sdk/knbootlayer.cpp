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
#include <QTimer>
#include <QTimeLine>

#include "knaudiomanager.h"
#include "knlocalemanager.h"

#include "knbootlayer.h"

KNBootLayer::KNBootLayer(QWidget *parent) : QWidget(parent),
    m_showText(new QTimeLine(667, this)),
    m_fadeBackground(new QTimeLine(667, this)),
    m_hideText(new QTimeLine(667, this))
{
    //Set properties.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0, 0, 0));
    pal.setColor(QPalette::WindowText, QColor(255, 255, 255, 0));
    setPalette(pal);
    //Configure the time line.
    m_showText->setUpdateInterval(16);
    m_showText->setFrameRange(0, 0xb4);
    connect(m_showText, &QTimeLine::frameChanged,
            this, &KNBootLayer::onUpdateTextColor);
    connect(m_showText, &QTimeLine::finished,
            m_fadeBackground, &QTimeLine::start);
    m_fadeBackground->setUpdateInterval(16);
    m_fadeBackground->setFrameRange(255, 0);
    connect(m_fadeBackground, &QTimeLine::frameChanged, [=](int frameValue)
    {
        //Update the background.
        QPalette pal=palette();
        pal.setColor(QPalette::Window, QColor(0, 0, 0, frameValue));
        setPalette(pal);
        //Repaint.
        update();
    });
    connect(m_fadeBackground, &QTimeLine::finished,
            m_hideText, &QTimeLine::start);
    m_hideText->setUpdateInterval(16);
    m_hideText->setFrameRange(0xb4, 0);
    connect(m_hideText, &QTimeLine::frameChanged,
            this, &KNBootLayer::onUpdateTextColor);
    connect(m_hideText, &QTimeLine::finished,
            [=]
    {
        //Emit the finish signal.
        emit requireStartup();
        //Hide the boot layer.
        hide();
    });
    //Update the background.
    update();
    //Update the font.
    updateFont();
    //Link the translation.
    knI18n->link(this, &KNBootLayer::retranslate);
    retranslate();
}

void KNBootLayer::startAnimation()
{
    //Start the timeline.
    QTimer::singleShot(2833, [=]
    {
        //Start the animation.
        m_showText->start();
        //Play the start up audio.
        knAudio->play(KNAudioManager::AudioStartUp);
    });
}

void KNBootLayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initialized the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the background.
    painter.fillRect(rect(), palette().color(QPalette::Window));
    painter.drawText(0, 0, static_cast<int>(0.825*static_cast<qreal>(width())),
                     height(), Qt::AlignRight | Qt::AlignVCenter,
                     m_bootText);
}

void KNBootLayer::resizeEvent(QResizeEvent *event)
{
    //Update the widget size.
    QWidget::resizeEvent(event);
    //Update the font size.
    updateFont();
}

void KNBootLayer::retranslate()
{
    //Update the text.
    m_bootText=tr("Kreogist Dev Team Entertainment");
}

void KNBootLayer::onUpdateTextColor(int frameValue)
{
    //Update the background.
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255, 255, 255, frameValue));
    setPalette(pal);
    //Repaint.
    update();
}

inline void KNBootLayer::updateFont()
{
    //Get the default font.
    QFont textFont=font();
    //Calculate the text font.
    int targetSize=qMax(static_cast<int>(0.026 * static_cast<qreal>(height())),
                        20);
    textFont.setFamily("SST Roman");
    textFont.setPixelSize(targetSize);
    textFont.setKerning(true);
    textFont.setStretch(124);
    //Set the font to the widget.
    setFont(textFont);
}
