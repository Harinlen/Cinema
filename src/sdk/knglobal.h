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
#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>

/*!
 * \def knGlobal
 * A global pointer referring to the unique global object.
 */
#define knGlobal (KNGlobal::instance())

class KNConfigure;
/*!
 * \brief The KNGlobal class is a global instance which contains several public
 * function. \n
 * You should only construct this class once. If you construct the KNGlobal more
 * than once, the other instance will become no use.\n
 * This global instance will also initial several singleton instance, and take
 * those managers' control.
 */
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    enum DefaultPath
    {
        KreogistDir,
        UserDataDir,
        ResourceDir,
        LibraryDir,
        DefaultDirCount
    };

    /*!
     * \brief Get the singleton instance of class KNGlobal.
     * \return The global instance of KNGlobal.
     */
    static KNGlobal *instance();

    /*!
     * \brief Initial the global class.
     * \param parent The parent object of the global class.
     */
    static void initial(QObject *parent = nullptr);

    /*!
     * \brief Get the specific directory path. The definition of each
     * directories.\n
     *   User Data Dir - This dir stores the configure file. This path cannot
     * be changed.\n
     * *   Windows: My documents/Kreogist/Cinema\n
     * *  Mac OS X: Documents/Kreogist/Cinema\n
     * *     Linux: ~/.kreogist/cinema\n
     *   Resource Dir - This dir stores the default system resource. Like some
     * official plugins. This path cannot be changed.\n
     * *   Windows: Application path\n
     * *  Mac OS X: Application path/../Resources\n
     * *     Linux: ~/.kreogist/cinema\n
     *   Library Dir - This dir stores the library data. This path can be
     * changed by user. The default path is the "Library" folder under the user
     * data path.\n
     * You should get the directory according to the DefaultPath enumerate.
     * \param index The directory enumerate number.
     * \return The path of the directory.
     */
    QString dirPath(int index);

    /*!
     * \brief Get the cache configure class.
     * \return The cache KNConfigure class.
     */
    KNConfigure *cacheConfigure();

    /*!
     * \brief Get the system configure class.
     * \return The system KNConfigure class.
     */
    KNConfigure *systemConfigure();

    /*!
     * \brief Get the user configure class.
     * \return The user KNConfigure class.
     */
    KNConfigure *userConfigure();

signals:

public slots:

private:
    static KNGlobal *ins;

    explicit KNGlobal(QObject *parent = nullptr);
    KNGlobal(const KNGlobal &);
    KNGlobal(KNGlobal &&);

    inline void initialDefaultDirPath();
    inline void initialInfrastrcture();

    QString m_dirPath[DefaultDirCount];
    KNConfigure *m_globalConfigure;
};

#endif // KNGLOBAL_H
