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
#ifndef KNMENUITEM_H
#define KNMENUITEM_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNMenuItem class provides the menu item that display in the menu
 * container.
 */
class KNMenuItem : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMenuItem widget.
     * \param parent The parent widget.
     * \param text The menu item display text.
     */
    explicit KNMenuItem(QWidget *parent = nullptr,
                        const QString &text=QString());

    /*!
     * \brief Whether the item is selected.
     * \return The menu item is selected.
     */
    bool isSelected() const;

signals:
    /*!
     * \brief When the item is marked as hovering, this signal will be emitted.
     */
    void hovering();

public slots:
    /*!
     * \brief Manually set whether the menu item is hovering.
     * \param isHover True for hover the item.
     */
    void setHovering(bool isHover, bool animated=true);

    /*!
     * \brief Set to display the selected mark.
     * \param isSelected Whether the menu item is clicked.
     */
    void setSelected(bool isSelected);

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) override;

    /*!
     * \brief Reimplmented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

private:
    inline void startAnimeFadeIn();
    inline void startAnimeFadeOut();
    QString m_text;
    QTimeLine *m_fadeAnimation;
    qreal m_opacity;
    int m_borderWidth;
    bool m_isSelected, m_isHover;
};

#endif // KNMENUITEM_H
