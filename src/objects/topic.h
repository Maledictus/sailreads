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
#include <QString>
#include <QObject>

#include "comment.h"
#include "groupfolder.h"

#include "../types.h"

namespace Sailreads
{
class Topic : public QObject
{
    Q_OBJECT

    QString m_Id;
    QString m_Title;
    quint64 m_CommentsCount;
    QDateTime m_LastCommentDate;
    QString m_ContextType;
    quint64 m_ContextId;
    UserPtr m_Author;
    GroupFolder m_GroupFolder;

    quint64 m_NewCommentsCount;
    quint64 m_CommentsPerPage;
    QDateTime m_UpdatedAt;
    QString m_SubjectType;
    GroupPtr m_Group;
    CountedItems<Comment> m_Comments;
    int m_UnreadCount;

    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString title READ GetTitle NOTIFY titleChanged)
    Q_PROPERTY(quint64 commentsCount READ GetCommentsCount NOTIFY commentsCountChanged)
    Q_PROPERTY(quint64 newCommentsCount READ GetNewCommentsCount NOTIFY newCommentsCountChanged)
    Q_PROPERTY(QDateTime updatedAt READ GetUpdateAt NOTIFY updatedAtChanged)
    Q_PROPERTY(QDateTime lastCommentDate READ GetLastCommentDate NOTIFY lastCommentDateChanged)
    Q_PROPERTY(User* author READ GetAuthor NOTIFY authorChanged)
    Q_PROPERTY(GroupFolder folder READ GetGroupFolder NOTIFY folderChanged)
    Q_PROPERTY(Group* group READ GetGroup NOTIFY groupChanged)
    Q_PROPERTY(int unreadCount READ GetUnreadCount NOTIFY unreadCountChanged)

public:
    Topic(QObject *parent = nullptr);
    ~Topic();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetTitle() const;
    void SetTitle(const QString& title);
    quint64 GetCommentsCount() const;
    void SetCommentsCount(quint64 count);
    QDateTime GetLastCommentDate() const;
    void SetLastCommentDate(const QDateTime& dt);
    QString GetContextType() const;
    void SetContextType(const QString& ctxType);
    quint64 GetContextId() const;
    void SetContextId(quint64 id);
    User* GetAuthor() const;
    UserPtr GetAuthorPtr() const;
    void SetAuthor(const UserPtr& user);
    GroupFolder GetGroupFolder() const;
    void SetGroupFolder(const GroupFolder& folder);

    quint64 GetNewCommentsCount() const;
    void SetNewCommentsCount(quint64 count);
    quint64 GetCommentsPerPage() const;
    void SetCommentsPerPage(quint64 count);
    QDateTime GetUpdateAt() const;
    void SetUpdatedAt(const QDateTime& dt);
    QString GetSubjectType() const;
    void SetSubjectType(const QString& title);
    Group* GetGroup() const;
    void SetGroup(const GroupPtr& group);
    CountedItems<Comment> GetComments() const;
    void SetComments(const CountedItems<Comment>& comments);
    int GetUnreadCount() const;
    void SetUnreadCount(int count);

signals:
    void idChanged();
    void titleChanged();
    void commentsCountChanged();
    void newCommentsCountChanged();
    void updatedAtChanged();
    void lastCommentDateChanged();
    void authorChanged();
    void folderChanged();
    void groupChanged();
    void unreadCountChanged();
};
} // namespace Sailreads
