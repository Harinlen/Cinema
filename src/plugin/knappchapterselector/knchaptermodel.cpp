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

using namespace ChapterUtil;

KNChapterModel::KNChapterModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int KNChapterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_chapterList.size();
}

QVariant KNChapterModel::data(const QModelIndex &index, int role) const
{
    //No valid data for invalid index.
    if(!index.isValid())
    {
        return QVariant();
    }
    //Get the chapter data.
    const ChapterData &data=m_chapterList.at(index.row());
    //Check the current row.
    switch(role)
    {
    case Qt::DisplayRole:
        //Chapter title.
        return data.title;
    case Qt::DecorationRole:
        //Chapter cover.
        return data.cover;
    }
    return QVariant();
}

const ChapterData &KNChapterModel::chapter(int index) const
{
    Q_ASSERT(index>-1 && index<m_chapterList.size());
    //Simply get the chapter data.
    return m_chapterList.at(index);
}

void KNChapterModel::setChapterList(const QList<ChapterData> &chapterList)
{
    //Reset the model first.
    reset();
    //Check the new chapter list.
    if(chapterList.isEmpty())
    {
        //No need to append the new list.
        return;
    }
    //Start insert the data.
    beginInsertRows(QModelIndex(), 0, chapterList.size()-1);
    m_chapterList=chapterList;
    endInsertRows();
    //Emit the new signal.
    emit rowCountChange(m_chapterList.size());
}

void KNChapterModel::reset()
{
    //Clear the original chapter list.
    if(!m_chapterList.isEmpty())
    {
        //Reset the model.
        beginResetModel();
        m_chapterList=QList<ChapterData>();
        endResetModel();
    }
}
