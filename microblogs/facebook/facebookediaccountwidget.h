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

#ifndef FACEBOOKEDITACCOUNT_H
#define FACEBOOKEDITACCOUNT_H

#include "editaccountwidget.h"
#include <QWidget>
#include "ui_facebookeditaccount_base.h"

namespace QOAuth {
class Interface;
}

class KJob;
class QProgressBar;
class FacebookAccount;
class FacebookMicroBlog;


class FacebookEditAccountWidget : public ChoqokEditAccountWidget, public Ui::FacebookEditAccountBase
{
    Q_OBJECT
public:
    FacebookEditAccountWidget(FacebookMicroBlog *microblog, FacebookAccount* account, QWidget *parent);

    /**
    * Destructor
    */
    virtual ~FacebookEditAccountWidget();

    virtual void showAuthenticationDialog();
    virtual bool validateData();
    /**
    * Create a new account if we are in the 'add account wizard',
    * otherwise update the existing account.
    * @Return new or modified account. OR 0L on failure.
    */
    virtual Choqok::Account *apply();

    /**
    * Returns the API Key, APP ID and APP SECRET
    */
    
    QString appID() const;
    QString apiKey() const;
    QString appSecret() const;

protected slots:
    virtual void userInfoJobDone();

protected:
    virtual void authenticationCancelled();
    virtual void authenticationDone(const QString& accessToken);
    virtual void updateUserName();
    
    void loadTimelinesTableState();
    void saveTimelinesTableState();
    virtual void getPinCode();
    void setAuthenticated(bool authenticated);
    bool isAuthenticated;
    FacebookMicroBlog *mBlog;
    FacebookAccount *mAccount;
    //QProgressBar *progress;
    QString username;
    QByteArray token;
    //QOAuth::Interface *qoauth;

};

#endif
