/*
Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include <QDateTime>
#include <QObject>
#include <QString>

#include "comment.h"
#include "bookshelf.h"
#include "../types.h"

namespace Sailreads
{
class Review: public QObject
{
    Q_OBJECT

    QString m_Id;
    BookPtr m_Book;
    int m_Rating;
    int m_LikesCount;
    BookShelves_t m_Shelves;
    QDateTime m_AddedDate;
    QDateTime m_UpdatedDate;
    QDateTime m_ReadDate;
    QDateTime m_StartedDate;
    int m_ReadCount;
    QString m_Body;
    quint64 m_CommentsCount;
    quint64 m_Owned;
    QUrl m_Url;
    UserPtr m_User;
    CountedItems<Comment> m_Comments;
    quint64 m_RatingId;
    bool m_SpoilerFlag;
    UserStatuses_t m_UserStatuses;
    ReadStatuses_t m_ReadStatuses;

    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)
    Q_PROPERTY(int rating READ GetRating NOTIFY ratingChanged)
    Q_PROPERTY(int likesCount READ GetLikesCount NOTIFY likesCountChanged)
    Q_PROPERTY(QDateTime addedDate READ GetAddedDate NOTIFY addedDateChanged)
    Q_PROPERTY(QDateTime updatedDate READ GetUpdatedDate NOTIFY updatedDateChanged)
    Q_PROPERTY(QDateTime readDate READ GetReadDate NOTIFY readDateChanged)
    Q_PROPERTY(QDateTime startedDate READ GetStartedDate NOTIFY startedDateChanged)
    Q_PROPERTY(int readCount READ GetReadCount NOTIFY readCountChanged)
    Q_PROPERTY(QString body READ GetBody NOTIFY bodyChanged)
    Q_PROPERTY(quint64 commentsCount READ GetCommentsCount NOTIFY commentsCountChanged)
    Q_PROPERTY(quint64 owned READ GetOwned NOTIFY ownedChanged)
    Q_PROPERTY(QUrl url READ GetUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList shelvesList READ GetShelvesList NOTIFY shelvesListChanged)
    Q_PROPERTY(QString exclusiveShelf READ GetExclusiveShelf NOTIFY exclusiveShelfChanged)
    Q_PROPERTY(User* user READ GetUser NOTIFY userChanged)
    Q_PROPERTY(QString shortDescription READ GetShortDescription NOTIFY shortDescriptionChanged)
    Q_PROPERTY(bool isLiked READ GetIsLiked NOTIFY isLikedChanged)
    Q_PROPERTY(quint64 ratingId READ GetRatingId NOTIFY ratingIdChanged)
    Q_PROPERTY(bool spoilerFlag READ GetSpoilerFlag NOTIFY spoilerFlagChanged)
    Q_PROPERTY(UserStatus* lastUserStatus READ GetLastUserStatus NOTIFY lastUserStatusChanged)

public:
    Review(QObject *parent = nullptr);
    ~Review();

    QString GetId() const;
    void SetId(const QString& id);
    Book* GetBook() const;
    BookPtr GetBookPtr() const;
    void SetBook(const BookPtr& book);
    int GetRating() const;
    void SetRating(int rating);
    int GetLikesCount() const;
    void SetLikesCount(int count);
    BookShelves_t GetShelves() const;
    void SetShelves(const BookShelves_t& shelves);
    QStringList GetShelvesList() const;
    QString GetExclusiveShelf() const;
    QDateTime GetAddedDate() const;
    void SetAddedDate(const QDateTime& addedDate);
    QDateTime GetUpdatedDate() const;
    void SetUpdatedDate(const QDateTime& updatedDate);
    QDateTime GetReadDate() const;
    void SetReadDate(const QDateTime& readDate);
    QDateTime GetStartedDate() const;
    void SetStartedDate(const QDateTime& startedDate);
    int GetReadCount() const;
    void SetReadCount(int readCount);
    QString GetBody() const;
    void SetBody(const QString& body);
    quint64 GetCommentsCount() const;
    void SetCommentsCount(const quint64& commentsCount);
    quint64 GetOwned() const;
    void SetOwned(const quint64& owned);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
    User* GetUser() const;
    UserPtr GetUserPtr() const;
    void SetUser(const UserPtr& user);
    CountedItems<Comment> GetComments() const;
    void SetComments(const CountedItems<Comment>& comments);
    QString GetShortDescription() const;
    bool GetIsLiked() const;
    void SetRatingId(quint64 ratingId);
    quint64 GetRatingId() const;
    bool GetSpoilerFlag() const;
    void SetSpoilerFlag(bool set);
    UserStatuses_t GetUserStatuses() const;
    void SetUserStatuses(const UserStatuses_t& userStatuses);
    UserStatus* GetLastUserStatus() const;
    ReadStatuses_t GetReadStatuses() const;
    void SetReadStatuses(const ReadStatuses_t& readStatues);

    void Update(Review *newReview);
    void Update(const ReviewPtr& newReview);

public slots:
    void updateShelves(Review *newReview);
    void updateShelves(const ReviewPtr& newReview);

signals:
    void idChanged();
    void bookChanged();
    void ratingChanged();
    void likesCountChanged();
    void addedDateChanged();
    void updatedDateChanged();
    void readDateChanged();
    void startedDateChanged();
    void readCountChanged();
    void bodyChanged();
    void commentsCountChanged();
    void ownedChanged();
    void urlChanged();
    void shelvesListChanged();
    void exclusiveShelfChanged();
    void userChanged();
    void shortDescriptionChanged();
    void isLikedChanged();
    void ratingIdChanged();
    void spoilerFlagChanged();
    void lastUserStatusChanged();
};
} // namespace Sailreads
