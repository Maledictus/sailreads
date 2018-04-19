/*
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

#pragma once

#include <QObject>
#include <QDateTime>
#include <QString>

#include "author.h"

namespace Sailreads
{
class Book
{
    Q_GADGET

    quint64 m_Id;
    QString m_ISBN;
    QString m_ISBN13;
    quint64 m_TextReviewsCount;
    QString m_Title;
    QString m_TitleWithoutSeries;
    QUrl m_ImageUrl;
    QUrl m_SmallImageUrl;
    QUrl m_LargeImageUrl;
    QUrl m_Link;
    int m_NumPages;
    QString m_Format;
    QString m_EditionInformation;
    QString m_Publisher;
    int m_PublicationYear;
    int m_PublicationMonth;
    int m_PublicationDay;
    qreal m_AverageRating;
    quint64 m_RatingsCount;
    QString m_Description;
    Authors_t m_Authors;
    int m_PublishedYear;
    quint64 m_WorkId;

    Q_PROPERTY(quint64 id READ GetId)
    Q_PROPERTY(QString isbn READ GetISBN)
    Q_PROPERTY(QString isbn13 READ GetISBN13)
    Q_PROPERTY(quint64 textReviewsCount READ GetTextReviewsCount)
    Q_PROPERTY(QString title READ GetTitle)
    Q_PROPERTY(QString titleWithoutSeries READ GetTitleWithoutSeries)
    Q_PROPERTY(QUrl imageUrl READ GetImageUrl)
    Q_PROPERTY(QUrl smallImageUrl READ GetSmallImageUrl)
    Q_PROPERTY(QUrl largeImageUrl READ GetLargeImageUrl)
    Q_PROPERTY(QUrl link READ GetLink)
    Q_PROPERTY(int numPages READ GetNumPages)
    Q_PROPERTY(QString format READ GetFormat)
    Q_PROPERTY(QString editionInformation READ GetEditionInformation)
    Q_PROPERTY(QString publisher READ GetPublisher)
    Q_PROPERTY(int publicationYear READ GetPublicationYear)
    Q_PROPERTY(int publicationMonth READ GetPublicationMonth)
    Q_PROPERTY(int publicationDay READ GetPublicationDay)
    Q_PROPERTY(qreal averageRating READ GetAverageRating)
    Q_PROPERTY(quint64 ratingsCount READ GetRatingsCount)
    Q_PROPERTY(QString description READ GetDescription)
    Q_PROPERTY(QString authors READ GetAuthorsString)
    Q_PROPERTY(int publishedYear READ GetPublishedYear)
    Q_PROPERTY(quint64 workId READ GetWorkId)

public:
    Book();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetISBN() const;
    void SetISBN(const QString& isbn);
    QString GetISBN13() const;
    void SetISBN13(const QString& isbn13);
    quint64 GetTextReviewsCount() const;
    void SetTextReviewsCount(const quint64& textReviewsCount);
    QString GetTitle() const;
    void SetTitle(const QString& title);
    QString GetTitleWithoutSeries() const;
    void SetTitleWithoutSeries(const QString& titleWithoutSeries);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& imageUrl);
    QUrl GetSmallImageUrl() const;
    void SetSmallImageUrl(const QUrl& smallImageUrl);
    QUrl GetLargeImageUrl() const;
    void SetLargeImageUrl(const QUrl& largeImageUrl);
    QUrl GetLink() const;
    void SetLink(const QUrl& link);
    int GetNumPages() const;
    void SetNumPages(int numPages);
    QString GetFormat() const;
    void SetFormat(const QString& format);
    QString GetEditionInformation() const;
    void SetEditionInformation(const QString& info);
    QString GetPublisher() const;
    void SetPublisher(const QString& publisher);
    int GetPublicationYear() const;
    void SetPublicationYear(int publicationYear);
    int GetPublicationMonth() const;
    void SetPublicationMonth(int publicationMonth);
    int GetPublicationDay() const;
    void SetPublicationDay(int publicationDay);
    qreal GetAverageRating() const;
    void SetAverageRating(const qreal& averageRating);
    quint64 GetRatingsCount() const;
    void SetRatingsCount(const quint64& ratingsCount);
    QString GetDescription() const;
    void SetDescription(const QString& description);
    Authors_t GetAuthors() const;
    void SetAuthors(const Authors_t& authors);
    QString GetAuthorsString() const;
    int GetPublishedYear() const;
    void SetPublishedYear(int publishedYear);
    quint64 GetWorkId() const;
    void SetWorkId(const quint64& workId);
};
} // namespace Sailreads
Q_DECLARE_METATYPE(Sailreads::Book)