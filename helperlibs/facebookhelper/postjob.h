/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef POSTJOB_H
#define POSTJOB_H

#include <libkfacebook/facebookjobs.h>
#include "postinfo.h"

/**
* A job to retrieve one or multiple posts from facebook and convert them into
* postInfo objects.
*/
class LIBKFACEBOOK_EXPORT PostJob : public FacebookGetIdJob
{
  Q_OBJECT
  public:
    /**
    * @brief Construct a postjob to retrieve multiple posts.
    *
    * @param postIds A list of ids of posts to retrieve.
    * @param accessToken The token to access data on facebook.
    */
    PostJob( const QStringList &postIds, const QString &accessToken );

    /**
    * @brief Construct a postjob to retrieve a single post from facebook.
    *
    * @param postId The id of the post to retrieve.
    * @param accessToken The token to accesss data on facebook.
    */
    PostJob( const QString &postId, const QString &accessToken );

    /**
    * @brief Return a list of all the posts that this job has retrieved
    *
    * @return List of pointers to postinfo objects
    */
    QList<PostInfoPtr> postInfo() const;

  private:
    void handleSingleData( const QVariant& data );

    QList<PostInfoPtr> mPostInfo;
};

#endif
