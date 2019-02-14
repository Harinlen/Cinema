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
#ifndef KNMENULAYER_H
#define KNMENULAYER_H

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNMenuLayer class provides the layer for displaying a menu.
 */
class KNMenuLayer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMenuLayer widget.
     * \param parent The parent widget.
     */
    explicit KNMenuLayer(QWidget *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Show the menu and the menu layer.
     */
    void showMenu();

    /*!
     * \brief Hide the menu and the menu layer.
     */
    void hideMenu();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) override;

private:
    QTimeLine *m_timeLine;
    int m_darkness;
};

#endif // KNMENULAYER_H
