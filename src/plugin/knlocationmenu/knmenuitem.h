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

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNMenuItem class provides the menu item that display in the menu
 * container.
 */
class KNMenuItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMenuItem widget.
     * \param text The menu item display text.
     * \param parent The parent widget.
     */
    explicit KNMenuItem(const QString &text = QString(),
                        QWidget *parent = nullptr);

    /*!
     * \brief Get the current item text.
     * \return The current item text.
     */
    QString text() const;

signals:
    /*!
     * \brief When the item is marked as hovering, this signal will be emitted.
     */
    void hovering();

public slots:
    /*!
     * \brief Set the text of the current menu item.
     * \param text The item text.
     */
    void setText(const QString &text);

protected:
    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) override;

    /*!
     * \brief Reimplmented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Manually set whether the menu item is hovering.
     * \param isHover True for hover the item.
     */
    void setHovering(bool isHover);

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
