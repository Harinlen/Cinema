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

#include "knappchapterselectorbase.h"

class KNChapterSelector;
/*!
 * \brief The KNAppChapterSelector class provides the panel for the chapter
 * selector and handling the keyword search box.
 */
class KNAppChapterSelector : public KNAppChapterSelectorBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAppChapterSelector widget.
     * \param parent The parent widget.
     */
    explicit KNAppChapterSelector(QWidget *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Reimplmented from
     * KNAppChapterSelectorBase::onLibraryPathChanged().
     */
    void onLibraryPathChanged(const QString &libraryPath) override;

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    KNChapterSelector *m_chapterSelector;
};

#endif // KNAPPCHAPTERSELECTOR_H
