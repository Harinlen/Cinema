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
#ifndef KNMAINAPPLICATIONHEADER_H
#define KNMAINAPPLICATIONHEADER_H

#include <QWidget>

class KNClockLabel;
/*!
 * \brief The KNMainApplicationHeader class provides the header information of
 * the system.
 */
class KNMainApplicationHeader : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainApplicationHeader widegt.
     * \param parent The parent widget.
     */
    explicit KNMainApplicationHeader(QWidget *parent = nullptr);

signals:

public slots:

private:
    KNClockLabel *m_clock;
};

#endif // KNMAINAPPLICATIONHEADER_H
