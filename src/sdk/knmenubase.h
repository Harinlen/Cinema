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
#ifndef KNMENUBASE_H
#define KNMENUBASE_H

#include <QWidget>

/*!
 * \brief The KNMenuBase class provides a widget interface for the menu using in
 * the menu layer.
 */
class KNMenuBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMenuBase widget.
     * \param parent The parent widget.
     */
    KNMenuBase(QWidget *parent = nullptr):QWidget(parent){}

signals:

public slots:
    /*!
     * \brief Set the menu and its item opacity.
     * \param opacity The opacity, the value is between 0 to 1.
     */
    virtual void setOpacity(qreal opacity) = 0;
};

#endif // KNMENUBASE_H
