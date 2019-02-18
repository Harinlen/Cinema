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
#ifndef KNCHAPTERSELECTOR_H
#define KNCHAPTERSELECTOR_H

#include <QDir>
#include <QPixmap>
#include <QThread>

#include "knchapterutil.h"

#include "knscrollarea.h"

#define LoadingFrameCount   20

class QTimeLine;
class KNChapterSearcher;
class KNChapterModel;
class KNProxyChapterModel;
/*!
 * \brief The KNChapterSelector class provides a container for selecting the
 * chapter. By providing a path, this widget will automatically load all the
 * videos under the path.
 */
class KNChapterSelector : public KNScrollArea
{
    Q_OBJECT
public:
    enum ScrollHint
    {
        PositionAtTop,
        PositionAtCenter,
        PositionAtBottom,
        EnsureVisible
    };

    /*!
     * \brief Construct a KNAppChapterSelector widget.
     * \param parent The parent widget.
     */
    explicit KNChapterSelector(QWidget *parent = nullptr);
    ~KNChapterSelector() override;

    /*!
     * \brief Get the current index of a specific visual point of the widget.
     * \param point The visual point.
     * \return The index of the model.
     */
    int indexAt(const QPoint &point) const;

    /*!
     * \brief Scroll to a specific index of item.
     * \param index The target item index of the display.
     * \param hint The scroll hint.
     */
    void scrollTo(int index, ScrollHint hint = EnsureVisible);

    /*!
     * \brief Get the item visual rect.
     * \param index The item index
     * \return The item visual rect of the current position.
     */
    QRect visualRect(int index) const;

signals:
    /*!
     * \brief Request for starting the searching.
     * \param dirPath The target directory path.
     */
    void requireStartSearch(QString dirPath);

    /*!
     * \brief Request to cancel the current searching.
     */
    void requireCancelSearch();

    /*!
     * \brief When item is selected, this signal will be emitted.
     * \param indexChange The selected item index.
     */
    void itemSelected(int indexChange);

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

    /*!
     * \brief Set the current selected index.
     * \param index The index of the item.
     */
    void setCurrentIndex(int index);

protected:
    /*!
     * \brief Reimplemented from KNAppChapterSelectorBase::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Reimplemented from KNAppChapterSelectorBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * \brief Reimplemented from KNAppChapterSelectorBase::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) override;

    /*!
     * \brief Reimplemented from KNAppChapterSelectorBase::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void retranslate();
    void onRowCountChange(int itemCount);
    void onSearchComplete(const QList<ChapterUtil::ChapterData> &chapterList);
    void startLoadDirectory();

private:
    enum SelectorState
    {
        StateNoDirectory,
        StateSelect,
        StateHiding,
        StateLoading,
        StateShowing
    };
    void validAndMoveToCurrent();
    void stopAllAnimations();
    void startHideAnimation();
    void startLoadingAnimation();
    void startShowAnimation();
    inline void selectCurrentItem();
    inline void updateParameters(int itemCount);
    inline int indexScrollBarValue(int index, ScrollHint hint);
    inline QRect itemContentRect(int index) const;

    QDir m_currentDir;
    QThread m_chapterSearchThread;
    QString m_addDirHint, m_loadingHint;
    QFont m_hintTextFont, m_loadingFont, m_labelFont;
    QLinearGradient m_titleGradient;
    QPixmap m_loadingRing[LoadingFrameCount];
    KNChapterModel *m_chapterModel;
    KNProxyChapterModel *m_proxyChapterModel;
    KNChapterSearcher *m_chapterSearcher;
    QTimer *m_loadingTimer;

    QTimeLine *m_scrollAnime, *m_hideAnimation;
    int m_maxColumnCount, m_itemWidth, m_itemHeight, m_itemSpacing,
        m_borderWidth, m_startX, m_currentIndex, m_currentState, m_loadingIndex,
        m_loadingWidth, m_textMargin;
    bool m_repeat;
};

#endif // KNCHAPTERSELECTOR_H
