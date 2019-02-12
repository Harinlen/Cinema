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
#ifndef KNBOOTLAYER_H
#define KNBOOTLAYER_H

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNBootLayer class provides the boot loader display of the system.
 */
class KNBootLayer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNBootLayer widget.
     * \param parent The parent widget.
     */
    explicit KNBootLayer(QWidget *parent = nullptr);

signals:
    /*!
     * \brief When the boot animation is finished, this signal will be emitted.
     */
    void requireStartup();

public slots:
    /*!
     * \brief Start the boot animation.
     */
    void startAnimation();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void retranslate();
    void onUpdateTextColor(int frameValue);

private:
    inline void updateFont();
    QTimeLine *m_showText, *m_fadeBackground, *m_hideText;
    QString m_bootText;
};

#endif // KNBOOTLAYER_H
