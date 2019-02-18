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
#include <QDir>

#include "knchaptersearcher.h"

using namespace ChapterUtil;

KNChapterSearcher::KNChapterSearcher(QObject *parent) : QObject(parent),
    m_coverSize(QSize(4, 4)),
    m_cancel(false),
    m_running(false)
{
}

void KNChapterSearcher::search(const QString &dirPath)
{
    //Clear the cancel flag and set the running flag.
    m_lock.lock();
    m_cancel=false;
    m_running=true;
    m_lock.unlock();
    //Get the sub dir list.
    QDir libraryDir(dirPath);
    QFileInfoList directoryList=libraryDir.entryInfoList();
    //Prepare the information list.
    QList<ChapterData> chapterList;
    chapterList.reserve(directoryList.size());
    //Find the possible chapters.
    for(auto i : directoryList)
    {
        //Check the cancel flag.
        m_lock.lock();
        if(m_cancel)
        {
            //Cancel the operation.
            emit searchCancelled();
            //Reset the flags.
            m_running=false;
            m_cancel=false;
            //Remove the lock.
            m_lock.unlock();
            //Mission complete.
            return;
        }
        m_lock.unlock();
        //Check the directory.
        if(i.fileName()=="." || i.fileName()==".." || !i.isDir())
        {
            //Ignore the ., .. and non-directory target.
            continue;
        }
        //The target should be a directory.
        ChapterData chapterData;
        chapterData.title=i.fileName();
        //Check and load the cover image.
        QPixmap &&coverImage=
                loadCover(QDir(i.absoluteFilePath()).filePath("cover.png"),
                          "png");
        if(coverImage.isNull())
        {
            coverImage=
                    loadCover(QDir(i.absoluteFilePath()).filePath("cover.jpg"),
                              "jpg");
        }
        if(!coverImage.isNull())
        {
            chapterData.cover=coverImage;
        }

        //Add the chapter data to the list.
        chapterList.append(chapterData);
    }
    //After all the chapter added, emit the finish signal.
    emit searchComplete(chapterList);
    //Reset the running and cancel flag.
    m_lock.lock();
    m_running=false;
    m_cancel=false;
    m_lock.unlock();
}

QSize KNChapterSearcher::coverSize() const
{
    return m_coverSize;
}

void KNChapterSearcher::setCoverSize(const QSize &coverSize)
{
    m_coverSize = coverSize;
}

bool KNChapterSearcher::cancel()
{
    //Lock the state.
    m_lock.lock();
    //Check the running state.
    if(m_running)
    {
        //Set the cancel flag.
        m_cancel=true;
    }
    bool needCancel=m_cancel;
    //Remove the lock
    m_lock.unlock();
    return needCancel;
}

void KNChapterSearcher::lockState()
{
    //Start locking the state.
    m_lock.lock();
}

void KNChapterSearcher::unlockState()
{
    //Unlock the state.
    m_lock.unlock();
}

QPixmap KNChapterSearcher::loadCover(const QString &coverPath,
                                       const char *format)
{
    if(QFileInfo::exists(coverPath))
    {
        //Try to load the image.
        QPixmap coverImage(coverPath, format);
        if(!coverImage.isNull())
        {
            //Scale the image to our size.
            QPixmap scaledImage=coverImage.scaled(
                        m_coverSize,
                        Qt::KeepAspectRatioByExpanding,
                        Qt::SmoothTransformation);
            if(scaledImage.size()!=m_coverSize)
            {
                scaledImage=scaledImage.copy(
                            (scaledImage.width()-m_coverSize.width())>>1,
                            (scaledImage.height()-m_coverSize.height())>>1,
                            m_coverSize.width(),
                            m_coverSize.height());
            }
            return scaledImage;
        }
    }
    return QPixmap();
}
