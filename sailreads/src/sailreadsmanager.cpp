/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "sailreadsmanager.h"

#include <QtDebug>

#include "objects/user.h"
#include "settings/accountsettings.h"
#include "goodreadsapi.h"
#include "userprofile.h"

namespace Sailreads
{
SailreadsManager::SailreadsManager(QObject *parent)
: QObject(parent)
, m_Api(new GoodReadsApi(this))
, m_IsBusy(false)
, m_IsLogged(false)
{
    MakeConnections();

    m_AccessToken = AccountSettings::Instance(this)->value("access_token").toString();
    m_AccessSecretToken = AccountSettings::Instance(this)->value("access_token_secret").toString();
    SetLogged(!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty());
}

SailreadsManager *SailreadsManager::Instance(QObject *parent)
{
    static SailreadsManager *sailreadsManager = nullptr;
    if (!sailreadsManager)
    {
        sailreadsManager = new SailreadsManager(parent);
    }
    return sailreadsManager;
}

bool SailreadsManager::GetBusy() const
{
    return m_IsBusy;
}

bool SailreadsManager::GetLogged() const
{
    return m_IsLogged;
}

User* SailreadsManager::GetAuthUser() const
{
    return m_AuthUser.get();
}

void SailreadsManager::MakeConnections()
{
    connect(m_Api, &GoodReadsApi::requestFinished,
            this, [=](){ SetBusy(false); });

    connect(m_Api, &GoodReadsApi::requestTokenChanged,
            this, &SailreadsManager::requestTokenChanged);
    connect(m_Api, &GoodReadsApi::accessTokensChanged,
            this,
            [=](const QString& accessToken, const QString& accessTokenSecret) {
                AccountSettings::Instance(this)->setValue("access_token", accessToken);
                AccountSettings::Instance(this)->setValue("access_token_secret", accessTokenSecret);
                m_AccessToken = accessToken;
                m_AccessSecretToken = accessTokenSecret;
                if (!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty()) {
                    m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
                }
                SetLogged(!accessToken.isEmpty() && !accessTokenSecret.isEmpty());
            });

    connect(m_Api, &GoodReadsApi::gotAuthUserInfo,
            this,
            [=](const quint64& id, const QString&, const QString&) {
                if (!m_AuthUser) {
                    m_AuthUser = std::make_shared<User>();
                }
                m_AuthUser->SetId(id);
                emit authUserChanged();
                emit gotAuthUserId(m_AuthUser->GetId());
            });
    connect(m_Api, &GoodReadsApi::gotUserProfile,
            this,
            [=](const UserPtr& profile) {
                if (!profile) {
                    return;
                }
                if (m_AuthUser && profile->GetId() == m_AuthUser->GetId()) {
                    qDebug() << "Authenticated profile updated";
                    m_AuthUser = profile;
                    emit authUserChanged();
                }
                emit gotUserProfile(profile);
                emit gotUserBookShelves(profile->GetId(), profile->GetBookShelves());
            });
    connect(m_Api, &GoodReadsApi::gotUserBookShelves,
            this, &SailreadsManager::gotUserBookShelves);
    connect(m_Api, &GoodReadsApi::bookShelfAdded,
            this, &SailreadsManager::bookShelfAdded);
    connect(m_Api, &GoodReadsApi::bookShelfEdited,
            this, &SailreadsManager::bookShelfEdited);

    connect(m_Api, &GoodReadsApi::gotReviews,
            this, &SailreadsManager::gotReviews);

    connect(m_Api, &GoodReadsApi::gotUserFriends,
            this, &SailreadsManager::gotUserFriends);

    connect(m_Api, &GoodReadsApi::gotUserGroups,
            this, &SailreadsManager::gotUserGroups);
    connect(m_Api, &GoodReadsApi::gotUserGroup,
            this, &SailreadsManager::gotUserGroup);
    connect(m_Api, &GoodReadsApi::gotFoundGroups,
            this, &SailreadsManager::gotFoundGroups);
    connect(m_Api, &GoodReadsApi::gotGroupMembers,
            this, &SailreadsManager::gotGroupMembers);
    connect(m_Api, &GoodReadsApi::gotGroupFolderTopics,
            this, &SailreadsManager::gotGroupFolderTopics);
    connect(m_Api, &GoodReadsApi::gotGroupFolderTopic,
            this, &SailreadsManager::gotGroupFolderTopic);
    connect(m_Api, &GoodReadsApi::gotBook,
            this, &SailreadsManager::gotBook);
}

void SailreadsManager::SetBusy(bool busy)
{
    m_IsBusy = busy;
    emit busyChanged();
}

void SailreadsManager::SetLogged(bool logged)
{
    m_IsLogged = logged;
    if (m_IsLogged) {
        m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
    }
    emit loggedChanged();
}

void SailreadsManager::obtainRequestToken()
{
    SetBusy(true);
    emit authProgressChanged(tr("Authorization user..."));
    m_Api->ObtainRequestToken();
}

void SailreadsManager::requestAccessToken()
{
    SetBusy(true);
    m_Api->RequestAccessToken();
}

void SailreadsManager::abortRequest()
{
    m_Api->AbortRequest();
}

void SailreadsManager::authenticateUser()
{
    SetBusy(true);
    emit authProgressChanged(tr("Authentication user..."));
    m_Api->AuthUser();
}

void SailreadsManager::getUserInfo(quint64 id)
{
    SetBusy(true);
    emit authProgressChanged(tr("Getting user profile..."));
    m_Api->GetUserInfo(id);
}

void SailreadsManager::getUpdates()
{
    SetBusy(true);
    m_Api->GetUpdates();
}

void SailreadsManager::loadBookShelves(quint64 id)
{
    SetBusy(true);
    m_Api->GetBookShelves(id);
}

void SailreadsManager::addBookShelf(const QString& name, bool exclusive)
{
    SetBusy(true);
    m_Api->AddBookShelf(name, exclusive);
}

void SailreadsManager::editBookShelf(quint64 id, const QString& name, bool exclusive)
{
    SetBusy(true);
    m_Api->EditBookShelf(id, name, exclusive);
}

void SailreadsManager::loadReviews(quint64 userId, const QString& bookShelf, int page,
        Qt::SortOrder order, const QString& sortField)
{
    SetBusy(true);
    m_Api->GetReviews(userId, bookShelf, sortField, order, page);
}

void SailreadsManager::loadBook(quint64 bookId)
{
    SetBusy(true);
    m_Api->GetBook(bookId);
}

void SailreadsManager::loadFriends(quint64 userId)
{
    SetBusy(true);
    m_Api->GetFriends(userId);
}

void SailreadsManager::loadGroups(quint64 userId)
{
    SetBusy(true);
    m_Api->GetGroups(userId);
}

void SailreadsManager::loadGroup(quint64 groupId, const QString& groupName)
{
    SetBusy(true);
    m_Api->GetGroup(groupId, groupName);
}

void SailreadsManager::searchGroup(const QString& text, int page)
{
    SetBusy(true);
    m_Api->SearchGroup(text, page);
}

void SailreadsManager::loadGroupMembers(quint64 groupId, int page)
{
    SetBusy(true);
    m_Api->GetGroupMembers(groupId, page);
}

void SailreadsManager::loadGroupFolderTopics(quint64 groupFolderId, quint64 groupId, int page)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopics(groupFolderId, groupId, page);
}

void SailreadsManager::loadGroupFolderTopic(quint64 topicId, int page)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopic(topicId, page);
}

void SailreadsManager::addNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        quint64 folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewTopic(topic, subject, subjectId, folderId, question, updateFeed, digest, comment);
}

void SailreadsManager::addNewComment(const QString& type, quint64 resourceId, const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewComment(type, resourceId, comment);
}
}
