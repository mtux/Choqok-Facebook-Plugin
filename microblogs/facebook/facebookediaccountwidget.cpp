/*
This file is part of Choqok, the KDE micro-blogging client

Copyright (C) 2008-2011 Mehrdad Momeny <mehrdad.momeny@gmail.com>

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

#include "facebookeditaccountwidget.h"
#include "facebookmicroblog.h"
#include "facebookaccount.h"
#include <KDebug>
#include <kio/jobclasses.h>
#include <kio/netaccess.h>
#include <kio/job.h>
#include <KMessageBox>
#include <QDomDocument>
#include <KToolInvocation>
#include <QProgressBar>
#include <accountmanager.h>
#include <choqoktools.h>
#include <QtOAuth/interface.h>
#include <QtOAuth/qoauth_namespace.h>
#include <kio/accessmanager.h>
#include <QCheckBox>
#include <KInputDialog>


FacebookEditAccountWidget::FacebookEditAccountWidget(FacebookMicroBlog *microblog, FacebookAccount* account, QWidget* parent) : ChoqokEditAccountWidget(account, parent), mAccount(account)
{
    setupUi(this);
    kcfg_basicAuth->hide();
    connect(kcfg_authorize, SIGNAL(clicked(bool)), this, SLOT(showAuthenticationDialog()));
    
   if(mAccount) {
        kcfg_alias->setText( mAccount->alias() );
        if(mAccount->accessToken().isEmpty() ) {
            setAuthenticated(false);
        } else {
            setAuthenticated(true);
            token = mAccount->accessToken();   
            username = mAccount->username();
        }
    } else {
        setAuthenticated(false);
        QString newAccountAlias = microblog->serviceName();
        QString servName = newAccountAlias;
        int counter = 1;
        while(Choqok::AccountManager::self()->findAccount(newAccountAlias)){
            newAccountAlias = QString("%1%2").arg(servName).arg(counter);
        counter++;
    }
        setAccount( mAccount = new FacebookAccount(microblog, newAccountAlias) );
        kcfg_alias->setText( newAccountAlias );
    }
    //loadTimelinesTableState();
    kcfg_alias->setFocus(Qt::OtherFocusReason);*/
}

FacebookEditAccountWidget::~FacebookEditAccountWidget()
{

}
void FacebookEditAccountWidget::showAuthenticationDialog()
{
  QStringList permissions;
  permissions << "offline_access"
	      << "publish_stream"
              << "friends_birthday"
              << "friends_website"
              << "friends_location"
              << "friends_work_history"
              << "friends_relationships"
              << "user_events"
              << "user_notes";
  AuthenticationDialog * const authDialog = new AuthenticationDialog( this );
  authDialog->setAppId( appID() );
  authDialog->setPermissions( permissions );
  connect( authDialog, SIGNAL(authenticated(QString)), this, SLOT(authenticationDone(QString)) );
  connect( authDialog, SIGNAL(canceled()), this, SLOT(authenticationCanceled()) );
  kcfg_authorize->setEnabled( false );
  authDialog->start();
}

void FacebookEditAccountWidget::authenticationCanceled()
{
  kcfg_authorize->setEnabled( true );
}

void FacebookEditAccountWidget::authenticationDone(const QString& accessToken)
{
  if ( mAccount->accessToken() != accessToken && !accessToken.isEmpty() ) {
    mTriggerSync = true;
  }
  mAccount->setAccessToken( accessToken );
  setAuthenticated(true);
  //updateAuthenticationWidgets();
  updateUserName();
}

void FacebookEditAccountWidget::updateUserName()
{
  if ( mAccount->userName().isEmpty() && ! mAccount->accessToken().isEmpty() ) {
    UserInfoJob * const job = new UserInfoJob( mAccount->accessToken() );
    connect( job, SIGNAL(result(KJob*)), this, SLOT(userInfoJobDone(KJob*)) );
    job->start();
  }
}

void FacebookEditAccountWidget::userInfoJobDone( KJob* job )
{
  UserInfoJob * const userInfoJob = dynamic_cast<UserInfoJob*>( job );
  Q_ASSERT( userInfoJob );
  if ( !userInfoJob->error() ) {
    mAccount->setUserName( userInfoJob->userInfo()->name() );
    setAuthenticated(true);
  } else {
    kWarning() << "Can't get user info: " << userInfoJob->errorText();
  }
}

bool FacebookEditAccountWidget::validateData()
{
    if(kcfg_alias->text().isEmpty() || !isAuthenticated )
        return false;
    else
        return true;
}


Choqok::Account* FacebookEditAccountWidget::apply()
{
    kDebug();
    mAccount->setAlias(kcfg_alias->text());
    mAccount->setUsername( username );
    mAccount->setAcessToken( token );
    mAccount->setUsingOAuth(true);
    saveTimelinesTableState();
    mAccount->writeConfig();
    return mAccount;
}


void FacebookEditAccountWidget::setAuthenticated(bool authenticated)
{
    isAuthenticated = authenticated;
    if(authenticated){
        kcfg_authorize->setIcon(KIcon("object-unlocked"));
        kcfg_authenticateLed->on();
	if ( mAccount->userName().isEmpty() ) {
	    kcfg_authenticateStatus->setText( i18n( "Authenticated." ) );
    } else {
      kcfg_authenticateStatus->setText( i18n( "Authenticated as <b>%1</b>.", mAccount->userName() ) );
    }
        kcfg_authenticateStatus->setText(i18n("Authenticated"));
    } else {
        kcfg_authorize->setIcon(KIcon("object-locked"));
        kcfg_authenticateLed->off();
        kcfg_authenticateStatus->setText(i18n("Not Authenticated"));
    }
}

/*
void FacebookEditAccountWidget::loadTimelinesTableState()
{
    foreach(const QString &timeline, mAccount->microblog()->timelineNames()){
        int newRow = timelinesTable->rowCount();
        timelinesTable->insertRow(newRow);
        timelinesTable->setItem(newRow, 0, new QTableWidgetItem(timeline));

        QCheckBox *enable = new QCheckBox ( timelinesTable );
        enable->setChecked ( mAccount->timelineNames().contains(timeline) );
        timelinesTable->setCellWidget ( newRow, 1, enable );
    }
}

void FacebookEditAccountWidget::saveTimelinesTableState()
{
    QStringList timelines;
    int rowCount = timelinesTable->rowCount();
    for(int i=0; i<rowCount; ++i){
        QCheckBox *enable = qobject_cast<QCheckBox*>(timelinesTable->cellWidget(i, 1));
        if(enable && enable->isChecked())
            timelines<<timelinesTable->item(i, 0)->text();
    }
    timelines.removeDuplicates();
    mAccount->setTimelineNames(timelines);
}
*/
QString Settings::apiKey() const
{
  return "698f95e6ad2c477359f75f344d12b899";
}

QString Settings::appID() const
{
  return "161169767282198";
}

QString Settings::appSecret() const
{
  return "343ea4a345c8926c1e00183e82ea9542";
}
#include "facebookconfigurewidget.moc"
