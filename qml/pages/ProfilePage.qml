﻿/*
The MIT License (MIT)

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

import QtQuick 2.2
import QtQml.Models 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: profilePage

    property alias userId: userProfile.userId
    property bool busy: sailreadsManager.busy && profilePage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    RemorsePopup { id: remorse }

    Component.onDestruction: {
        userProfile.cancelRequest()
    }

    UserProfile {
        id: userProfile
    }

    BaseProxyModel {
        id: bookShelvesModel

        limit: 3
        filterRole: BookShelvesModel.Exclusive
        filterRegExp: new RegExp("true")
        dynamicSortFilter: true
        sourceModel: BookShelvesModel {
            userId: profilePage.userId
        }
    }

    SilicaFlickable {
        id: profileView

        anchors.fill: parent

        contentHeight: content.height + header.height

        PageHeader {
            id: header
            title: userProfile.user ? userProfile.user.userName : ""
            description: userProfile.user ? userProfile.user.nickName : ""
        }

        Column {
            id: content

            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
            }

            PullDownMenu {
                busy: profilePage.busy
                MenuItem {
                    visible: sailreadsManager.authUser.id === userId
                    text: qsTr("Settings")
                    onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                }

                MenuItem {
                    visible: userProfile.user &&
                        (sailreadsManager.authUser.id !== userId) &&
                        !userProfile.user.isFriend &&
                        userProfile.user.friendStatus != User.FriendRequestSent
                    text: userProfile.user ?
                        (userProfile.user.isFollowing ? qsTr("Unfollow user") : qsTr("Follow user")) :
                        ""
                    onClicked: {
                        if (userProfile.user) {
                            userProfile.user.isFollowing ?
                                sailreadsManager.unfollowUser(userId) :
                                sailreadsManager.followUser(userId)
                        }
                    }
                }

                MenuItem {
                    visible: userProfile.user &&
                        (sailreadsManager.authUser.id !== userId) &&
                        !userProfile.user.isFriend
                    enabled: userProfile.user &&
                            (userProfile.user.friendStatus == User.NotAFriend ||
                                userProfile.user.friendStatus == User.FriendRequestReceived)
                    text: {
                        if (userProfile.user &&
                                userProfile.user.friendStatus == User.FriendRequestSent) {
                            return qsTr("Friend request was sent")
                        }
                        else if (userProfile.user &&
                                userProfile.user.friendStatus == User.FriendRequestReceived) {
                            return qsTr("Accept friend request")
                        }

                        return qsTr("Add as a friend")
                    }
                    onClicked: {
                        if (userProfile.user &&
                                userProfile.user.friendStatus == User.FriendRequestReceived) {
                            sailreadsManager.confirmFriendRequest(userProfile.user.friendRequestId, true)
                        }
                        else {
                            sailreadsManager.addAsFriend(userId)
                        }
                    }
                }

                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        userProfile.loadProfile()
                    }
                }
            }

            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                spacing: Theme.paddingMedium

                BaseImage {
                    id: avatarImage
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    source: userProfile.user ? userProfile.user.avatarUrl : ""
                    indicator.size: BusyIndicatorSize.Medium
                }

                Column {
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Details")
                        value: Utils.getDetailsInfoString(
                                   userProfile.user ? userProfile.user.age : 0,
                                   userProfile.user ? userProfile.user.gender : "",
                                   userProfile.user ? userProfile.user.location : "")
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Joined")
                        value: userProfile.user ? userProfile.user.joinedDate : ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Last active")
                        value: userProfile.user ? userProfile.user.lastUpdateDate : ""
                    }
                }
            }

            Column {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Interests")
                    value: userProfile.user ? userProfile.user.interests : ""
                }
                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Favorite books")
                    value: userProfile.user ? userProfile.user.favoriteBooksDesc : ""
                }

                Label {
                    property string key: qsTr("Favorite authors")
                    property string value: userProfile.user ? userProfile.user.favoriteAutors : ""

                    width: parent.width
                    color: Theme.highlightColor
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    textFormat: Text.RichText
                    visible: value !== ""
                    maximumLineCount: 3
                    text: "<font color=\"" + Theme.secondaryHighlightColor + "\">" +
                            key + "</font> " +
                            "<style>a:link{color:" + Theme.primaryColor + ";}</style>" +
                            value.replace(/\n/g, '<br>')
                    onLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                            { authorId: Number(link).toFixed() })
                }

                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("About me")
                    value: userProfile.user ? userProfile.user.about : ""
                }
            }

            Item {
                width: 1
                height: Theme.paddingMedium
            }

            MoreButton {
                id: bookShelvesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Bookshelves")
                counter: userProfile.user ? userProfile.user.bookShelvesCount : 0
                busy: profilePage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BookShelvesPage.qml"), {
                        userId: userProfile.user ? userProfile.user.id : "",
                        userName: userProfile.user ? userProfile.user.userName : "",
                    })
                }
            }

            SilicaListView {
                id: bookshelvesView

                width: parent.width
                height: contentHeight
                delegate: BookShelfListItem {
                    shelfName: bookShelfName
                    shelfBooksCount: bookShelfBooksCount
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("BookReviewsPage.qml"),
                                { userId: profilePage.userId, userName: userProfile.user.userName,
                                    bookShelfId: bookShelfId, bookShelf: bookShelfName })
                    }
                }

                model: bookShelvesModel

                BusyIndicator {
                    size: BusyIndicatorSize.Large
                    anchors.centerIn: parent
                    running: profilePage.busy
                    visible: running
                }
            }

            MoreButton {
                id: friendsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Friends")
                counter: userProfile.user ? userProfile.user.friendsCount : 0
                busy: profilePage.busy
                enabled: !busy
                visible: userProfile.user ? userProfile.user.friendsCount > 0 : false
                onClicked: {
                    if (sailreadsManager.authUser && sailreadsManager.authUser.id === userId &&
                            applicationSettings.showFriendsUpdates) {
                        pageStack.push(Qt.resolvedUrl("FriendsUpdatesPage.qml"))
                        return
                    }

                    pageStack.push(Qt.resolvedUrl("FriendsPage.qml"), { userId: profilePage.userId })
                }
            }

            MoreButton {
                id: groupsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Groups")
                counter: userProfile.user ? userProfile.user.groupsCount : 0
                busy: profilePage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("GroupsPage.qml"), { userId: profilePage.userId })
                }
            }

            MoreButton {
                id: messagesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Messages")
                counter: ""
                visible: sailreadsManager.authUser && userId === sailreadsManager.authUser.id
                busy: profilePage.busy
                enabled: !busy
                onClicked: pageStack.push(Qt.resolvedUrl("MessagesPage.qml"))
            }

            MoreButton {
                id: notificationButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Notifications")
                counter: ""
                visible: sailreadsManager.authUser && userId === sailreadsManager.authUser.id
                busy: profilePage.busy
                enabled: !busy
                onClicked: pageStack.push(Qt.resolvedUrl("NotificationsPage.qml"))
            }

            SectionHeader {
                text: qsTr("Recent updates")
                visible: (sailreadsManager.authUser && sailreadsManager.authUser.id !== userId) ||
                    applicationSettings.showYourRecentUpdates
            }

            SilicaListView {
                id: recentUpdatesView

                visible: (sailreadsManager.authUser && sailreadsManager.authUser.id !== userId) ||
                    applicationSettings.showYourRecentUpdates

                width: parent.width
                height: contentHeight
                delegate: UpdateListItem {
                    imageUrl: updateImageUrl
                    headerText: updateHeaderText
                    date: Qt.formatDateTime(updateUpdatedDate)
                    onLinkActivated: mainWindow.openPageFromUrl(link)
                    onClicked: mainWindow.openPageFromUrl(updateLink)
                }

                model: ProfileUpdatesModel {
                    userId: profilePage.userId
                }

                BusyIndicator {
                    size: BusyIndicatorSize.Large
                    anchors.centerIn: parent
                    running: profilePage.busy
                    visible: running
                }
            }
        }

        VerticalScrollDecorator {}
    }
}
