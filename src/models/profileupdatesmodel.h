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

#include "updatesmodel.h"

namespace Sailreads
{
class ProfileUpdatesModel : public UpdatesModel
{
    Q_OBJECT

    quint64 m_UserId;
    Q_PROPERTY(quint64 userId READ GetUserId WRITE SetUserId NOTIFY userIdChanged)

public:
    ProfileUpdatesModel(QObject *parent = nullptr);

    quint64 GetUserId() const;
    void SetUserId(quint64 userId);

    virtual void classBegin() override;
    virtual void componentComplete() override;

private slots:
    void handleGotUpdates(quint64 userId, const Updates_t& updates);
public slots:
    void cancelRequest();
signals:
    void userIdChanged();
};

} // namespace Sailreads
