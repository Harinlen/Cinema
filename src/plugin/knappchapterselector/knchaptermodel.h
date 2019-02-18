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
#ifndef KNCHAPTERMODEL_H
#define KNCHAPTERMODEL_H

#include "knchapterutil.h"

#include <QAbstractListModel>

/*!
 * \brief The KNChapterModel class provides a model for getting
 */
class KNChapterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNChapterModel object.
     * \param parent The parent object.
     */
    explicit KNChapterModel(QObject *parent = nullptr);

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    override;

    /*!
     * \brief Get the chapter data.
     * \param index The chapter index data.
     * \return The chapter information.
     */
    const ChapterUtil::ChapterData &chapter(int index) const;

signals:
    /*!
     * \brief When the row count is changed, this signal will be emitted.
     * \param rowCount The new row count.
     */
    void rowCountChange(int rowCount);

public slots:
    /*!
     * \brief Set the new chapter list.
     * \param chapterList New allocated chapter list.
     */
    void setChapterList(const QList<ChapterUtil::ChapterData> &chapterList);

    /*!
     * \brief Reset the entire model.
     */
    void reset();

private:
    QList<ChapterUtil::ChapterData> m_chapterList;
};

#endif // KNCHAPTERMODEL_H
