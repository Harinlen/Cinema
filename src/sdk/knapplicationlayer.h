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
#ifndef KNAPPLICATIONLAYER_H
#define KNAPPLICATIONLAYER_H

#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class KNHWidgetSwitcher;
class KNMainApplicationHeader;
/*!
 * \brief The KNApplicationLayer class provides the layer for application to
 * draw the application data.
 */
class KNApplicationLayer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNApplicationLayer widget.
     * \param parent The parent widget.
     */
    explicit KNApplicationLayer(QWidget *parent = nullptr);

signals:
    /*!
     * \brief Request for loading configure.
     */
    void requireLoadConfigure();

public slots:
    /*!
     * \brief Append an application widget to the container.
     * \param widget The application widget.
     */
    void appendAppWidget(QWidget *widget);

    /*!
     * \brief When the layer is shown, this animation will be executed.
     */
    void showAnimation();

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    inline void setRange(QPropertyAnimation *animation,
                         const QPoint &start, const QPoint &end);
    inline QPropertyAnimation *generateAnimation(QWidget *target);
    KNHWidgetSwitcher *m_container;
    KNMainApplicationHeader *m_header;
    QPropertyAnimation *m_headerAnimation, *m_containerAnimation;
    QParallelAnimationGroup *m_showAnimation;
};

#endif // KNAPPLICATIONLAYER_H
