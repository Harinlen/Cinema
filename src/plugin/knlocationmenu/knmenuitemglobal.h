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
#ifndef KNMENUITEMGLOBAL_H
#define KNMENUITEMGLOBAL_H

#include <QPixmap>

#include <QObject>

/*!
 * \brief The KNMenuItemGlobal class provides the shared resource for the menu
 * items.
 */
class KNMenuItemGlobal : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the menu item global instance.
     * \return The menu item global instance pointer.
     */
    static KNMenuItemGlobal *instance();

    /*!
     * \brief Initial the menu item global.
     * \param parent The parent object pointer.
     */
    static void initial(QObject *parent = nullptr);

    /*!
     * \brief Get the tick icon pixmap.
     * \return The tick icon.
     */
    QPixmap tickIcon() const;

signals:

public slots:

private:
    static KNMenuItemGlobal *ins;

    explicit KNMenuItemGlobal(QObject *parent = nullptr);
    KNMenuItemGlobal(const KNMenuItemGlobal &);
    KNMenuItemGlobal(KNMenuItemGlobal &&);

    QPixmap m_tick;
};

#endif // KNMENUITEMGLOBAL_H
