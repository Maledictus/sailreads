﻿/*
The MIT License(MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include <memory>

#include <QDomDocument>
#include <QDomElement>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPair>

#include "objects/bookshelf.h"
#include "objects/comment.h"
#include "objects/friend.h"
#include "objects/friendrequest.h"
#include "objects/friendrecommendation.h"
#include "objects/groupfolder.h"
#include "objects/groupmember.h"
#include "objects/quote.h"
#include "types.h"

namespace Sailreads
{
namespace RpcUtils
{
namespace Parser
{
UserPtr ParseUser(const QDomElement& element);
BookShelf ParseBookShelf(const QDomElement& element);
FavoriteAuthors_t ParseFavoriteAuthors(const QDomElement& faElement);
GroupMember ParseGroupMember(const QDomElement& element);
GroupFolder ParseGroupFolder(const QDomElement& element);
GroupPtr ParseGroup(const QDomElement& element);
Friend ParseFriend(const QDomElement& element);
FriendRequest ParseFriendRequest(const QDomElement& element);
FriendRecommendation ParseFriendRecommendation(const QDomElement& element);
TopicPtr ParseTopic(const QDomElement& element);
Comment ParseComment(const QDomElement& element);
BookPtr ParseBook(const QDomElement& element);
ReviewPtr ParseReview(const QDomElement& element);
ReviewPtr ParseReviewFromShelf(const QDomElement& element);
ReviewInfo ParseReviewInfo(const QDomElement& element);
AuthorPtr ParseAuthor(const QDomElement& element);
WorkPtr ParseWork(const QDomElement& element);
SeriesPtr ParseSeries(const QDomElement& element);
SeriesWorkPtr ParseSeriesWork(const QDomElement& element);
MessagePtr ParseMessage(const QDomElement& element);
QString ParseResourceId(const QDomElement& element);
NotificationPtr ParseNotification(const QDomElement& element);
UpdatePtr ParseUpdate(const QDomElement& element);
ReadStatusPtr ParseReadStatus(const QDomElement& element);
UserStatusPtr ParseUserStatus(const QDomElement& element);
RecommendationPtr ParseRecommendation(const QDomElement& element);
BookPtr ParseBookFromWork(const QDomElement& element);

Quote ParseQuoteFromArticle(const QString& article);

GroupMembers_t ParseGroupMembers(const QDomElement& element);
GroupFolders_t ParseGroupFolders(const QDomElement& element);
BookShelves_t ParseBookShelves(const QDomElement& element);
Groups_t ParseGroups(const QDomElement& element);
Friends_t ParseFriends(const QDomElement& element);
FriendsRequests_t ParseFriendsRequests(const QDomElement& element);
FriendsRecommendations_t ParseFriendsRecommendations(const QDomElement& element);
Topics_t ParseTopics(const QDomElement& element);
Authors_t ParseAuthors(const QDomElement& element);
CountedItems<Comment> ParseComments(const QDomElement& element);
CountedItems<ReviewPtr> ParseReviews(const QDomElement& element);
SeriesWorks_t ParseSeriesWorks(const QDomElement& element);
Series_t ParseSeriesFromSeriesWorks(const QDomElement& element);
Books_t ParseBooksList(const QDomElement& element);
CountedItems<BookPtr> ParseBooks(const QDomElement& element);
Reviews_t ParseReviewsList(const QDomElement& element);
Series_t ParseAuthorSeries(const QDomElement& element);
Messages_t ParseMessagesList(const QDomElement& element);
CountedItems<MessagePtr> ParseMessages(const QDomElement& element);
Notifications_t ParseNotifications(const QDomElement& element);
Users_t ParseUsers(const QDomElement& element);
Updates_t ParseUpdatesList(const QDomElement& element);
Books_t ParseBooksFromWorksList(const QDomElement& element);
UserStatuses_t ParseUserStatusesList(const QDomElement& element);
ReadStatuses_t ParseReadStatusesList(const QDomElement& element);

Quotes_t ParseJsonQuotesContent(const QString& htmlContent);

UserPtr ParseUser(const QDomDocument& doc);
CountedItems<BookShelf> ParseBookShelves(const QDomDocument& doc);
CountedItems<GroupPtr> ParseGroups(const QDomDocument& doc);
CountedItems<Friend> ParseFriends(const QDomDocument& doc);
CountedItems<Friend> ParseFollowings(const QDomDocument& doc);
CountedItems<Friend> ParseFollowers(const QDomDocument& doc);
GroupPtr ParseGroup(const QDomDocument& doc);
GroupMembers_t ParseGroupMembers(const QDomDocument& doc);
CountedItems<TopicPtr> ParseGroupFolderTopics(const QDomDocument& doc);
TopicPtr ParseGroupFolderTopic(const QDomDocument& doc);
Comment ParseComment(const QDomDocument& doc);
QPair<quint64, CountedItems<ReviewPtr>> ParseReviews(const QDomDocument& doc);
CountedItems<ReviewPtr> ParseFoundReviews(const QDomDocument& doc);
BookPtr ParseBook(const QDomDocument& doc);
SeriesPtr ParseSeries(const QDomDocument& doc);
AuthorPtr ParseAuthor(const QDomDocument& doc);
CountedItems<BookPtr> ParseAuthorBooks(const QDomDocument& doc);
ReviewPtr ParseReview(const QDomDocument& doc);
ReviewInfo ParseReviewInfo(const QDomDocument& doc);
Series_t ParseAuthorSeries(const QDomDocument& doc);
CountedItems<BookPtr> ParseBookEditions(const QDomDocument& doc);
CountedItems<MessagePtr> ParseMessages(const QDomDocument& doc);
MessagePtr ParseMessage(const QDomDocument& doc);
Notifications_t ParseNotifications(const QDomDocument& doc);
Updates_t ParseUpdates(const QDomDocument& doc);
ReadStatusPtr ParseReadStatus(const QDomDocument& doc);
CountedItems<Comment> ParseComments(const QDomDocument& doc);
UserStatusPtr ParseUserStatus(const QDomDocument& doc);
RecommendationPtr ParseRecommendation(const QDomDocument& doc);
CountedItems<FriendRequest> ParseFriendsRequests(const QDomDocument& doc);
CountedItems<FriendRecommendation> ParseFriendsRecommendations(const QDomDocument& doc);
ReviewPtr ParseBookShelfAddedReview(const QDomDocument& doc);
Reviews_t ParseBookShelfAddedReviews(const QDomDocument& doc);
CountedItems<BookPtr> ParseFoundBooks(const QDomDocument& doc);
Quotes_t ParseUserQuotes(const QDomDocument& doc);

PageCountedItems<Quote> ParseJsonQuotes(const QJsonDocument& doc);
}
}
}
