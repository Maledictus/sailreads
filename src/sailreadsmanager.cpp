/*
Copyright (c) 2014 Oleg Linkin <maledictusdemagog@gmail.com>

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
#include <algorithm>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QtDebug>
#include "friendsmodel.h"
#include "goodreadsapi.h"
#include "localstorage.h"
#include "notificationsmodel.h"
#include "recentupdatesmodel.h"
#include "userprofile.h"

namespace SailReads
{
	SailreadsManager::SailreadsManager (QQuickView *view, QObject *parent)
	: QObject (parent)
	, MainView_ (view)
	, GoodreadsApi_ (new GoodreadsApi (this))
	, LocalStorage_ (new LocalStorage (this))
	, UpdatesModel_ (new RecentUpdatesModel (this))
	, NotificationsModel_ (new NotificationsModel (this))
	, FriendsModel_ (new FriendsModel (this))
	{
		connect (GoodreadsApi_,
				SIGNAL (requestInProcessChanged ()),
				this,
				SIGNAL (requestInProcessChanged ()));
		connect (GoodreadsApi_,
				SIGNAL (gotAuthUserID (QString)),
				this,
				SLOT (handleGotAuthUserID (QString)));
		connect (GoodreadsApi_,
				SIGNAL (gotUserProfile (UserProfile*)),
				this,
				SLOT (handleGotUserProfile (UserProfile*)));
		connect (GoodreadsApi_,
				SIGNAL (gotRecentUpdates (Updates_t)),
				this,
				SLOT (handleGotRecentUpdates (Updates_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotNotifications (Notifications_t)),
				this,
				SLOT (handleGotNotifications (Notifications_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotFriends (Friends_t)),
				this,
				SLOT (handleGotFriends (Friends_t)));
	}

	void SailreadsManager::Init ()
	{
		connect (MainView_->rootObject (),
				SIGNAL (applicationAuthorized (bool)),
				this,
				SLOT (handleApplicationAuthorized (bool)));
		connect (MainView_->rootObject (),
				SIGNAL (refreshUpdates ()),
				this,
				SLOT (handleRefreshUpdates ()));
		connect (MainView_->rootObject (),
				SIGNAL (requestUserProfile (QString)),
				this,
				SLOT (handleRequestUserProfile (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestNotifications ()),
				this,
				SLOT (handleRequestNotifications ()));
		connect (MainView_->rootObject (),
				SIGNAL (requestFriendsList (QString)),
				this,
				SLOT (handleRequestFriendsList (QString)));

		MainView_->rootContext ()->setContextProperty ("updatesModel", UpdatesModel_);
		MainView_->rootContext ()->setContextProperty ("notificationsModel", NotificationsModel_);
		MainView_->rootContext ()->setContextProperty ("friendsModel", FriendsModel_);

		AccessToken_ = LocalStorage_->GetValue ("AccessToken");
		AccessTokenSecret_ = LocalStorage_->GetValue ("AccessTokenSecret");

		if (AccessToken_.isEmpty () || AccessTokenSecret_.isEmpty ())
			AuthorizeApplication ();
		else
			RequestAuthUserId ();
	}

	bool SailreadsManager::IsRequestInProcess () const
	{
		return GoodreadsApi_->IsRequestInProcess ();
	}

	void SailreadsManager::AuthorizeApplication ()
	{
		int failTimes = 3;
		QUrl authUrl;
		while (failTimes && authUrl.isEmpty ())
		{
			authUrl = GoodreadsApi_->GetAuthorizationUrl ();
			if (authUrl.isEmpty ())
				--failTimes;
		}

		if (authUrl.isEmpty ())
		{
			//TODO make notifiacation
			qWarning () << Q_FUNC_INFO
					<< "Unbale to get authorization page. Please try later";
			return;
		}

		QMetaObject::invokeMethod (MainView_->rootObject (),
				"authorizeApplication",
				Q_ARG (QVariant, QVariant::fromValue (authUrl)));
	}

	void SailreadsManager::RequestAuthUserId ()
	{
		GoodreadsApi_->RequestAuthUserID (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleApplicationAuthorized (bool authorized)
	{
		if (authorized)
		{
			const auto& tokens = GoodreadsApi_->GetAccessTokens ();
			if (tokens.first.isEmpty () || tokens.second.isEmpty ())
			{
				//TODO notification
				qWarning () << Q_FUNC_INFO
						<<  "Unable to get access token and access token secret";
				return;
			}

			LocalStorage_->SetValue ("AccessToken", tokens.first);
			LocalStorage_->SetValue ("AccessTokenSecret", tokens.second);
			AccessToken_ = tokens.first;
			AccessTokenSecret_ = tokens.second;

			RequestAuthUserId ();
		}
	}

	void SailreadsManager::handleRefreshUpdates ()
	{
		UpdatesModel_->Clear();
		GoodreadsApi_->RequestFriendsUpdates (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleRequestUserProfile (const QString& id)
	{
		GoodreadsApi_->RequestUserInfo (id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleRequestNotifications ()
	{
		NotificationsModel_->Clear ();
		GoodreadsApi_->RequestNotifications (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleRequestFriendsList (const QString& id)
	{
		FriendsModel_->Clear ();
		GoodreadsApi_->RequestFriends (AccessToken_,
				AccessTokenSecret_, id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleGotAuthUserID (const QString& id)
	{
		AuthUserID_ = id;
		GoodreadsApi_->RequestFriendsUpdates (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleGotUserProfile (UserProfile *profile)
	{
		QMetaObject::invokeMethod (MainView_->rootObject (),
				"setUserProfile",
				Q_ARG (QVariant, QVariant::fromValue<QObject*> (profile)));
	}

	void SailreadsManager::handleGotRecentUpdates (const Updates_t& updates)
	{
		UpdatesModel_->Clear ();
		UpdatesModel_->AddItems (updates);
	}

	void SailreadsManager::handleGotNotifications (const Notifications_t& notifications)
	{
		int newNotificationsCount = std::count_if (notifications.begin (), notifications.end (),
				[] (decltype (notifications.first ()) notification)
				{
					return !notification.Read_;
				});
		QMetaObject::invokeMethod (MainView_->rootObject (),
				"setUnreadNotificationsCount",
				Q_ARG (QVariant, QVariant::fromValue (newNotificationsCount)));

		if (newNotificationsCount)
		{
			//TODO
			qDebug () << "you have unread notificaion";
		}

		NotificationsModel_->Clear ();
		NotificationsModel_->AddItems (notifications);
	}

	void SailreadsManager::handleGotFriends (const Friends_t& friends)
	{
		FriendsModel_->Clear ();
		FriendsModel_->AddItems (friends);
	}
}
