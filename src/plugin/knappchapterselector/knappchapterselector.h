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
#ifndef KNAPPCHAPTERSELECTOR_H
#define KNAPPCHAPTERSELECTOR_H

#include <QDir>
#include <QThread>

#include <QScrollArea>

class QTimeLine;
class KNChapterSearcher;
class KNChapterModel;
class KNProxyChapterModel;
/*!
 * \brief The KNAppChapterSelector class provides a container for selecting the
 * chapter. By providing a path, this widget will automatically load all the
 * videos under the path.
 */
class KNAppChapterSelector : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAppChapterSelector widget.
     * \param parent The parent widget.
     */
    explicit KNAppChapterSelector(QWidget *parent = nullptr);
    ~KNAppChapterSelector();

signals:
    void requireStartSearch(QString dirPath);
    void requireCancelSearch();

public slots:
    /*!
     * \brief Set the current display directory.
     * \param dirPath The new directory path.
     */
    void setCurrentDir(const QString &dirPath);

    /*!
     * \brief Reset the directory to no directory state.
     */
    void reset();

private:
    enum SelectorState
    {
        StateNoDirectory,
        StateSelect,
        StateHiding,
        StateLoading,
        StateShowing
    };

    inline void startHideAnimation();

    QDir m_currentDir;
    QThread m_chapterSearchThread;
    KNChapterModel *m_chapterModel;
    KNProxyChapterModel *m_proxyChapterModel;
    KNChapterSearcher *m_chapterSearcher;

    QTimeLine *m_hideAnimation;
    int m_currentState;
};

#endif // KNAPPCHAPTERSELECTOR_H
