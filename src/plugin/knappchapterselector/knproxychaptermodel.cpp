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
#include "knchaptermodel.h"

#include "knproxychaptermodel.h"

KNProxyChapterModel::KNProxyChapterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

const ChapterUtil::ChapterData &KNProxyChapterModel::chapter(
        int proxyIndex) const
{
    //Get source index.
    QModelIndex sourceIndex=mapToSource(index(proxyIndex, 0));
    //Get the original data.
    if(sourceIndex.isValid())
    {
        return static_cast<KNChapterModel *>(sourceModel())->chapter(
                    sourceIndex.row());
    }
    return m_nullData;
}

bool KNProxyChapterModel::lessThan(const QModelIndex &source_left,
                                   const QModelIndex &source_right) const
{
    //Sort the display role.
    return source_left.data(Qt::DisplayRole).toString() <
            source_right.data(Qt::DisplayRole).toString();
}

bool KNProxyChapterModel::filterAcceptsRow(
        int source_row,
        const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    // No key word, all matches.
    if(m_keyword.isEmpty())
    {
        return true;
    }
    //Filter the content.
    if(sourceModel())
    {
        //Get the display content.
        return sourceModel()->index(source_row, 0).data(
                    Qt::DisplayRole).toString().contains(m_keyword);
    }
    return false;
}
