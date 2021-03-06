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
#include "postinfo.h"

#include <libkfacebook/util.h>

#include <KDebug>
#include <KLocalizedString>
#include <KPIMUtils/LinkLocator>

void PostInfo::setId( const QString &id)
{
  mId = id;
}

KMime::Message::Ptr PostInfo::asPost() const
{
  KMime::Message * const post = new KMime::Message();

  QString m = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n";
  m += "<html><head></head><body>\n";
  m += message();
  m += "</body>";

  post->setBody( m.toAscii() );
  post->date()->fromUnicodeString( updatedTime().toString(KDateTime::RFCDateDay), "utf-8" );
  post->contentType()->fromUnicodeString( "text/html", "utf-8" );
  post->from()->fromUnicodeString( "you@facebook", "utf-8" );

  post->assemble();

  return KMime::Message::Ptr(post);
}

QString PostInfo::id() const
{
  return mId;
}

void PostInfo::setFrom( const QString &from)
{
  mFrom = from;
}

QString PostInfo::from() const
{
  return mFrom;
}


void PostInfo::setMessage( const QString &message)
{
  mMessage = message;
}

QString PostInfo::message() const
{
  return mMessage;
}

void PostInfo::setCreatedTimeString( const QString &createdTime )
{
  mCreatedTime = createdTime;
}

QString PostInfo::createdTimeString() const
{
  return mCreatedTime;
}

KDateTime PostInfo::createdTime() const
{
  return facebookTimeToKDateTime(mCreatedTime);
}

void PostInfo::setUpdatedTimeString( const QString &updatedTime )
{
  mUpdatedTime = updatedTime;
}

QString PostInfo::updatedTimeString() const
{
  return mUpdatedTime;
}

KDateTime PostInfo::updatedTime() const
{
  return facebookTimeToKDateTime(mUpdatedTime);
}
