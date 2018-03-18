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

#pragma once

#include <QObject>
#include <QFutureWatcher>
#include <QUrl>
#include <QUrlQuery>

namespace Sailreads
{

class OAuthWrapper : public QObject
{
    Q_OBJECT

    const QString m_ConsumerKey;
    const QString m_ConsumerSecretKey;
    const QUrl m_BaseUrl;

public:
    explicit OAuthWrapper (const QString& consumerKey,
            const QString& consumerSecretKey, const QUrl& baseUrl,
            QObject *parent = 0);

    QUrl GetRequestTokenUrl() const;
    QUrl GetAccessTokenUrl(const QString& requestToken, const QString& requestTokenSecret) const;

    QPair<QUrl, QByteArray> MakeSignedUrl(const QString& accessToken, const QString& accessTokenSecret,
            const QUrl& url, const QString& method = "GET");
};
}