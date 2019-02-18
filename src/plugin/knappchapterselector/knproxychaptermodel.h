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
#ifndef KNPROXYCHAPTERMODEL_H
#define KNPROXYCHAPTERMODEL_H

#include "knchapterutil.h"

#include <QSortFilterProxyModel>

/*!
 * \brief The KNProxyChapterModel class provides a class for sort and search the
 * chapter model data.
 */
class KNProxyChapterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNProxyChapterModel model
     * \param parent The parent object.
     */
    explicit KNProxyChapterModel(QObject *parent = nullptr);

    /*!
     * \brief Get the chapter information by providing its proxy index.
     * \param proxyIndex The chapter proxy index.
     * \return The chapter data.
     */
    const ChapterUtil::ChapterData &chapter(int proxyIndex) const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QSortFilterProxyModel::lessThan().
     */
    bool lessThan(const QModelIndex &source_left,
                  const QModelIndex &source_right) const override;

    /*!
     * \brief Reimplemented from QSortFilterProxyModel::filterAcceptsRow().
     */
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;

private:
    ChapterUtil::ChapterData m_nullData;
    QString m_keyword;
};

#endif // KNPROXYCHAPTERMODEL_H
