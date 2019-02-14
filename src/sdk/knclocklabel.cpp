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
#include <QTime>
#include <QTimer>

#include "knclocklabel.h"

KNClockLabel::KNClockLabel(QWidget *parent) :
    QWidget(parent),
    m_clockUpdate(new QTimer(this)),
    m_numWidth(0),
    m_numHeight(0),
    m_12hour(true)
{
    //Update the font size.
    updateFontSize();
    //Update the timer.
    m_clockUpdate->setInterval(500);
    connect(m_clockUpdate, &QTimer::timeout, [=]{update();});
}

void KNClockLabel::setFont(const QFont &font)
{
    //Set the widget font.
    QWidget::setFont(font);
    //Update the font size.
    updateFontSize();
}

void KNClockLabel::updateFontSize()
{
    int targetWidth=-1, targetHeight=-1;
    //Get the size of the biggest char length.
    for(int i=0; i<10; ++i)
    {
        //Get the size of the char.
        QChar numChar('0'+i);
        //Save the num size.
        QSize numSize=fontMetrics().size(Qt::TextSingleLine, numChar);
        //Update the width and height.
        targetWidth=qMax(targetWidth, numSize.width());
        targetHeight=qMax(targetHeight, numSize.height());
    }
    //Update the target width and height.
    m_numWidth=targetWidth;
    m_numHeight=targetHeight;
    //Update the width, "11:11 AM" for 8 chars.
    setFixedWidth(m_numWidth * 8);
}

void KNClockLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing, true);

    QString timeText=getCurrentTime();
    int x=width()-timeText.size()*m_numWidth, i;
    //Loop and check the text.
    for(i=0; i<timeText.size(); ++i)
    {
        //Draw the char.
        painter.drawText(x, 0, m_numWidth, height(), Qt::AlignCenter,
                         timeText.mid(i, 1));
        if(timeText.at(i+1)==' ')
        {
            break;
        }
        //Move to the next position.
        x+=m_numWidth;
    }
    //Draw the left content.
    painter.drawText(x, 0, width()-x, height(),
                     Qt::AlignRight | Qt::AlignVCenter,
                     timeText.mid(i+1));
}

QString KNClockLabel::getCurrentTime()
{
    //Get the current hour time.
    QTime currentTime = QTime::currentTime();
    QString timeText, ampmText;
    if(m_12hour && currentTime.hour() > 12)
    {
        timeText = QString::number(currentTime.hour() - 12);
        ampmText = " PM";
    }
    else
    {
        timeText = QString::number(currentTime.hour());
        ampmText = " AM";
    }
    //Get the minuate
    timeText.append(":" + currentTime.toString("mm"));
    //Whether we need AM/PM
    if(m_12hour)
    {
        timeText.append(ampmText);
    }
    return timeText;
}
