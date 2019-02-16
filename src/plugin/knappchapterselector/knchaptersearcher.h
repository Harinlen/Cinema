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
#ifndef KNCHAPTERSEARCHER_H
#define KNCHAPTERSEARCHER_H

#include <QSize>

#include "knchapterutil.h"

#include <QObject>

/*!
 * \brief The KNChapterSearcher class provides a searcher working in an
 * independent threads for finding all the valid chapters under a directory.
 */
class KNChapterSearcher : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNChapterSearcher object.
     * \param parent The parent object.
     */
    explicit KNChapterSearcher(QObject *parent = nullptr);

    /*!
     * \brief Get the cover scaled size.
     * \return The cover target scaled size.
     */
    QSize coverSize() const;

signals:
    /*!
     * \brief When the search is cancelled, this signal will be emitted.
     */
    void searchCancelled();

    /*!
     * \brief When the search is completed, this signal will be emitted.
     * \param chapterList The new chapter list.
     */
    void searchComplete(QList<ChapterUtil::ChapterData> chapterList);

public slots:
    /*!
     * \brief Start to search the directory path.
     * \param dirPath The target directory path.
     */
    void search(const QString &dirPath);

    /*!
     * \brief Set the cover scaling size.
     * \param coverSize The new cover size.
     */
    void setCoverSize(const QSize &coverSize);

    /*!
     * \brief Cancel the thread running.
     */
    void cancel();

private:
    inline QPixmap loadCover(const QString &coverPath, const char *format);
    QSize m_coverSize;
    bool m_cancel, m_running;
};

#endif // KNCHAPTERSEARCHER_H
