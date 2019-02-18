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
#ifndef KNBACKGROUND_H
#define KNBACKGROUND_H

#include <QWidget>

class QLabel;
/*!
 * \brief The KNBackground class provides the ability for displaying an image or
 * a video as the background.
 */
class KNBackground : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNBackground widget.
     * \param parent The parent widget.
     */
    explicit KNBackground(QWidget *parent = nullptr);

signals:

public slots:
    /*!
     * \brief Set background as a static image.
     * \param image The target image.
     */
    void setBackground(const QPixmap &image);

    /*!
     * \brief Set the background as a video.
     * \param videoPath The target video path.
     */
    void setBackground(const QString &videoPath);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    inline void updateStreamSize();
    QPixmap m_defaultBackground;
    QHash<int, QPixmap> m_scaledMap;
    QPixmap m_frame, m_scaledFrame;
    QStringList m_frameNameList;
    QTimer *m_videoTimer;
    int m_frameIndex;
};

#endif // KNBACKGROUND_H
