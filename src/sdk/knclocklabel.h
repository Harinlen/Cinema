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
#ifndef KNCLOCKLABEL_H
#define KNCLOCKLABEL_H

#include <QWidget>

/*!
 * \brief The KNClockLabel class provides the clock that would update every
 * second.
 */
class KNClockLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNClockLabel widget.
     * \param parent The parent widget.
     */
    explicit KNClockLabel(QWidget *parent = nullptr);

    /*!
     * \brief Reimplemented from QWidget::setFont().
     */
    void setFont(const QFont &font);

signals:

public slots:
    /*!
     * \brief Update the font size according to the current font.
     */
    void updateFontSize();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

private:
    inline QString getCurrentTime();
    QTimer *m_clockUpdate;
    int m_numWidth, m_numHeight;
    bool m_12hour;
};

#endif // KNCLOCKLABEL_H
