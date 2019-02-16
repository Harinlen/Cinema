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
#ifndef KNLOCATIONMENU_H
#define KNLOCATIONMENU_H

#include "knmenubase.h"

/*!
 * \brief The KNLocationMenu class provides a menu for managing the location.
 */
class KNLocationMenu : public KNMenuBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLocationMenu widget.
     * \param parent The parent widget.
     */
    explicit KNLocationMenu(QWidget *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMenuBase::setOpacity().
     */
    void setOpacity(qreal opacity) override;

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QLinearGradient m_backgroundGradient;
    qreal m_opacity;
};

#endif // KNLOCATIONMENU_H
