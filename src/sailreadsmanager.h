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

#pragma once

#include <QObject>
#include "structures.h"

class QQuickView;

namespace SailReads
{
	class GoodreadsApi;
	class LocalStorage;
	class RecentUpdatesModel;

	class SailreadsManager : public QObject
	{
		Q_OBJECT

		QQuickView *MainView_;
		GoodreadsApi *GoodreadsApi_;
		LocalStorage *LocalStorage_;

		QString AccessToken_;
		QString AccessTokenSecret_;
		QString AuthUserID_;

		RecentUpdatesModel *UpdatesModel_;

		Q_PROPERTY (bool requestInProcess READ IsRequestInProcess NOTIFY requestInProcessChanged)

	public:
		explicit SailreadsManager (QQuickView *view, QObject *parent = 0);

		void Init ();
		bool IsRequestInProcess () const;
	private:
		void AuthorizeApplication ();
		void RequestAuthUserId ();

	private slots:
		void handleApplicationAuthorized (bool authorized);
		void handleRefreshUpdates ();

		void handleGotAuthUserID (const QString& id);
		void handleGotUserProfile (const UserProfile& profile);
		void handleGotRecentUpdates (const Updates_t& updates);

	signals:
		void requestInProcessChanged ();
	};
}
