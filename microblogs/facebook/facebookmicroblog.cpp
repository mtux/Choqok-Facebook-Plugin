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
#include <facebookhelper/postinfo.h>
#include <facebookhelper/postjob.h>
K_PLUGIN_FACTORY( MyPluginFactory, registerPlugin < FacebookPlugin > (); )
K_EXPORT_PLUGIN( MyPluginFactory( "choqok_facebook" ) )

FacebookMicroblog::FacebookMicroblog( QObject *parent, const char *name, const QList<QVariant> & args ): Choqok::Plugin( MyPluginFactory::componentData(), parent, name )
{
//...
}
 
FacebookMicroblog::~FacebookMicroblog()
{
}


void FacebookMicroblog::createPost(Choqok::Acount* the Account, Choqok::Post* post) 
{
    kDebug()<<"Creating a new Post";
    FacebookAccount* acc = qobject_cast<FacebookAccount*>(theAccount);
    PostAddJob* job = new PostAddJob(post->content, acc->accessToken());
    mJobsAccount.insert(job, acc);
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotCreatePost(KJob*)) );
    job->start();
 
}

void FacebookMicroblog::slotCreatePost(KJob* job)
{
    FacebookAccount* acc = mJobsAccount.take(job);
    Choqok::Post* post = mJobsPost.take(job);
    emit postCreated ( acc, post );
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
        return new FacebookEditAccountWidget(this, acc, parent);
    else{
        kDebug()<<"Account passed here was not a valid Facebook Account!";
        return 0L;
    }
    
Choqok::TimelineInfo* timelineInfo(const QString& timelineName)
{
  if(timelineName == "Home") {
   
    Choqok::TimelineInfo * info = new Choqok::TimelineInfo;
    info->name = i18nc("Timeline Name", "Home");
    info->description = i18nc("Timeline description", "Your Facebook Home");
    info->icon = "user-home";
    return info;
  } else {
    
    kError()<<"timelineName is not valid!";
    return 0;
  }
}

QList< Choqok::Post* > FacebookMicroblog::loadTimeline(Choqok::Account* account, const QString& timelineName)
{
    kDebug()<<timelineName;
    QList< Choqok::Post* > list;
    QString fileName = Choqok::AccountManager::generatePostBackupFileName(account->alias(), timelineName);
    KConfig postsBackup( "choqok/" + fileName, KConfig::NoGlobals, "data" );
    QStringList tmpList = postsBackup.groupList();

    QList<QDateTime> groupList;
    foreach(const QString &str, tmpList)
        groupList.append(QDateTime::fromString(str) );
    qSort(groupList);
    int count = groupList.count();
    if( count ) {
        Choqok::Post *st = 0;
        for ( int i = 0; i < count; ++i ) {
            st = new Choqok::Post;
            KConfigGroup grp( &postsBackup, groupList[i].toString() );
            st->creationDateTime = grp.readEntry( "creationDateTime", QDateTime::currentDateTime() );
            st->postId = grp.readEntry( "postId", QString() );
            st->content = grp.readEntry( "text", QString() );
            st->author.userId = grp.readEntry( "authorId", QString() );
            st->author.userName = grp.readEntry( "authorUserName", QString() );
            st->author.realName = grp.readEntry( "authorRealName", QString() );
            //st->isRead = grp.readEntry("isRead", true);

            list.append( st );
        }
    }
    return list;
}


void FacebookMicroblog::saveTimeline(Choqok::Account* account, const QString& timelineName, const QList< Choqok::UI::PostWidget* >& timeline)
{
    kDebug();
    QString fileName = Choqok::AccountManager::generatePostBackupFileName(account->alias(), timelineName);
    KConfig postsBackup( "choqok/" + fileName, KConfig::NoGlobals, "data" );

    ///Clear previous data:
    QStringList prevList = postsBackup.groupList();
    int c = prevList.count();
    if ( c > 0 ) {
        for ( int i = 0; i < c; ++i ) {
            postsBackup.deleteGroup( prevList[i] );
        }
    }
    QList< Choqok::UI::PostWidget *>::const_iterator it, endIt = timeline.constEnd();
    for ( it = timeline.constBegin(); it != endIt; ++it ) {
        const Choqok::Post *post = &((*it)->currentPost());
        KConfigGroup grp( &postsBackup, post->creationDateTime.toString() );
        grp.writeEntry( "creationDateTime", post->creationDateTime );
        grp.writeEntry( "postId", post->postId.toString() );
        grp.writeEntry( "text", post->content );
        grp.writeEntry( "authorId", post->author.userId.toString() );
        grp.writeEntry( "authorUserName", post->author.userName );
        grp.writeEntry( "authorRealName", post->author.realName );
        //grp.writeEntry( "isRead" , post->isRead );
    }
    postsBackup.sync();
	if(Choqok::Application::isShuttingDown())
		emit readyForUnload();
}

void FacebookMicroblog::updateTimelines(Choqok::Account * theAccount)
{
  kDebug();
    FacebookAccount* acc = qobject_cast<FacebookAccount*>(theAccount);
    if(!acc){
        kError()<<"FacebookMicroblog::updateTimelines: acc is not an FacebookAccount";
        return;
    }
    
    PostsListJob * job = new PostsListJob(acc->accessToken);
    mJobsAccount.insert(job, acc);
    connect( job, SIGNAL(result(KJob*)), this, SLOT(slotTimeLineLoaded(KJob*)) );
    job->start();
    
}

void FacebookMicroblog::slotTimeLineLoaded(KJob *job)
{
  kDebug();
    FacebookAccount* acc = mJobsAccount.take(job);
    if ( job->error() ) {
        kDebug() << "Job Error: " << job->errorString();
        emit error( theAccount, CommunicationError, i18n("Timeline update failed, %1", job->errorString()), Low );
     
    }else {
        emit timelineDataReceived( acc, "Home", job->posts() );
    }
}
void FacebookMicroblog::aboutToUnload()
{
    emit saveTimelines();
}

}