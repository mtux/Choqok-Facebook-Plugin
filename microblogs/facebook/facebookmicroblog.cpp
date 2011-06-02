/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2010-2011 Mehrdad Momeny <mehrdad.momeny@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see http://www.gnu.org/licenses/
*/

#include "facebookmicroblog.h"
#include <KAboutData>
#include <KGenericFactory>
#include "facebookaccount.h"
#include <KMessageBox>
#include <libkfacebook/postinfo.h>
#include<libkfacebook/postjob.h>
K_PLUGIN_FACTORY( MyPluginFactory, registerPlugin < FacebookPlugin > (); )
K_EXPORT_PLUGIN( MyPluginFactory( "choqok_facebook" ) )

FacebookMicroblog::FacebookMicroblog( QObject *parent, const char *name, const QList<QVariant> & args ): Choqok::Plugin( MyPluginFactory::componentData(), parent, name )
{
//...
}
 
FacebookMicroblog::~FacebookMicroblog()
{
}

void FacebookMicroblog::setAccessToken (const QString& acessToken)
{
  mAcessToken = acessToken;
}

QString FacebookMicroblog::accessToken() const
{
  return mAccessToken;
}
void FacebookMicroblog::createPost(Choqok::Acount* the Account, Choqok::Post* post) 
{
    kDebug()<<"Creating a new Post";
    FacebookAccount* acc = qobject_cast<FacebookAccount*>(theAccount);
    PostAddJob job(post->content, mAccessToken);
    mJobsAccount.insert(&job, acc);
    job.start();
    emit pos
}

void FacebookMicroblog::abortCreatePost(Choqok::Account* theAccount, Choqok::Post* post)
{
    FacebookAccount* acc = qobject_cast<FacebookAccount*>(theAccount); 
    Q_UNUSED(post);
    
    FacebookJob* job = mJobsAccount.key(acc);
    
    if(job)
      job->abort();
}

void FacebookMicroblog::fetchPost(Choqok::Account* theAccount, Choqok::Post* post)
{
    Q_UNUSED(theAccount);
    Q_UNUSED(post);
    KMessageBox::sorry(choqokMainWindow, i18n("Not Supported"));
}

void FacebookMicroblog::removePost(Choqok::Account* theAccount, Choqok::Post* post)
{
    Q_UNUSED(theAccount);
    Q_UNUSED(post);
    KMessageBox::sorry(choqokMainWindow, i18n("Not Supported"));
}

void FacebookMicroblog::createNewAccount(const QString& alias)
{
FacebookAccount *acc = qobject_cast<FacebookAccount*>( Choqok::AccountManager::self()->findAccount(alias) );
    if(!acc) {
        return new FacebookAccount(this, alias);
    } else {
        return 0;//If there's an account with this alias, So We can't create a new one
    }
}

ChoqokEditAccountWidget* FacebookMicroblog::createEditAccountWidget(Choqok::Account* account, QWidget* parent)
{
    kDebug();
    FacebookAccount *acc = qobject_cast<FacebookAccount*>(account);
    if(acc || !account)
        return new FacebookConfigureWidget(this, acc, parent);
    else{
        kDebug()<<"Account passed here was not a valid Facebook Account!";
        return 0L;
    }
}