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
#ifndef KNLOCATIONMENU_H
#define KNLOCATIONMENU_H

#include "knmenubase.h"

class QBoxLayout;
class QSignalMapper;
class KNConfigure;
class KNMenuItem;
/*!
 * \brief The KNLocationMenu class provides a menu for managing the location. It
 * supports for adding and switching locations.
 */
class KNLocationMenu : public KNMenuBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLocationMenu widget.
     * \param parent The parent widget.
     */
    explicit KNLocationMenu(QWidget *parent = nullptr);

    /*!
     * \brief Add menu item.
     * \param item The menu item widget pointer.
     */
    void addItem(KNMenuItem *item);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMenuBase::setOpacity().
     */
    void setOpacity(qreal opacity) override;

    /*!
     * \brief Reimplemented from KNMenuBase::loadFromConfigure().
     */
    void loadFromConfigure() override;

    /*!
     * \brief Reimplemented from KNMenuBase::prepareHover().
     */
    void prepareHover() override;

    /*!
     * \brief Set the current hover index.
     * \param index The index of the item.
     * \param animated Whether the select operation needs an animation.
     */
    void setCurrentHover(int index, bool animated=true);

    /*!
     * \brief Set the current loading index.
     * \param index The index of the selected item.
     */
    void setCurrentIndex(int index);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void retranslate();
    void onItemHover();
    void onItemClicked();

private:
    QLinearGradient m_backgroundGradient;
    QStringList m_pathList;
    QList<KNMenuItem *> m_itemList;
    QBoxLayout *m_mainLayout;
    KNMenuItem *m_addPathItem;
    KNConfigure *m_configure;
    qreal m_opacity;
    int m_currentHover, m_currentIndex;
};

#endif // KNLOCATIONMENU_H
