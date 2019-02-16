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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class KNBackground;
class KNApplicationLayer;
class KNMenuBase;
class KNMenuLayer;
class KNBootLayer;
class KNConfigure;
/*!
 * \brief The KNMainWindow class provides the main window for the application.
 */
class KNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a MainWindow widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindow(QWidget *parent = nullptr);

signals:
    /*!
     * \brief Ask to save the configurations to the hard drive.
     */
    void requireSaveConfigure();


public slots:
    /*!
     * \brief Append an application widget to the container.
     * \param widget The application widget.
     */
    void appendAppWidget(QWidget *widget);

    /*!
     * \brief Set the main menu widget.
     * \param menuWidget The menu widget.
     */
    void setMenuWidget(KNMenuBase *menuWidget);

protected:
    /*!
     * \brief Reimplemented from QMainWindow::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * \brief Reimplemented from QMainWindow::showEvent().
     */
    void showEvent(QShowEvent *event) override;

    /*!
     * \brief Reimplemented from QMainWindow::closeEvent().
     */
    void closeEvent(QCloseEvent *event) override;

    /*!
     * \brief Reimplemented from QMainWindow::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event) override;

private:
    KNConfigure *m_configure;
    KNBackground *m_background;
    KNApplicationLayer *m_applicationLayer;
    KNMenuLayer *m_menuLayer;
    KNBootLayer *m_bootLayer;
};

#endif // MAINWINDOW_H
