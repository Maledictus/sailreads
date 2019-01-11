/*
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
import Sailfish.Silica 1.0
import harbour.sailreads 1.0
import "../components"

Page {
    id: messagesPage

    property bool busy: sailreadsManager.busy && messagesPage.status === PageStatus.Active
    property MessagesFolder messagesFolder: inboxMessagesFolder

    property MessagesFolder inboxMessagesFolder: MessagesFolder {
        name: qsTr("Inbox")
        key: "inbox"
    }
    property MessagesFolder savedBookmarksFilter: MessagesFolder {
        name: qsTr("Saved")
        key: "saved"
    }
    property MessagesFolder sentBookmarksFilter: MessagesFolder {
        name: qsTr("Sent")
        key: "sent"
    }
    property MessagesFolder trashBookmarksFilter: MessagesFolder {
        name: qsTr("Trash")
        key: "trash"
    }
    property variant messagesFolders: [
        inboxMessagesFolder,
        savedBookmarksFilter,
        sentBookmarksFilter,
        trashBookmarksFilter
    ]

    onMessagesFolderChanged: {
        messagesModel.folder = messagesFolder.key
    }

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    MessagesModel {
        id: messagesModel
    }

    SilicaListView {
        id: messagesView
        anchors.fill: parent
        clip: true

        PullDownMenu {
            MenuItem {
                text: qsTr("Select folder")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("MessagesFolderSelectorPage.qml"),
                            { parentPage: messagesPage, model: messagesFolders })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: sailreadsManager.loadMessages(messagesModel.folder)
            }
        }

        header: PageHeader {
            title: "Messages: %1".arg(messagesFolder.name)
        }

        model: messagesModel
        cacheBuffer: messagesPage.height
        ViewPlaceholder {
            enabled: !sailreadsManager.busy && messagesView.count === 0
            text: qsTr("There are no messages in this folder. Pull down to refresh")
        }

        function fetchMoreIfNeeded() {
            if (!messagesPage.busy &&
                    messagesModel.hasMore &&
                    indexAt(contentX, contentY + height) > messagesModel.rowCount() - 2) {
                messagesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        RemorseItem { id: remorse }

        delegate: ListItem {
            id: listItem

            width: parent.width
            contentHeight: column.height + separator.height + Theme.paddingMedium
            clip: true

            Column {
                id: column
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                PosterHeaderItem {
                    width: parent.width
                    posterAvatar: messageFromUser.avatarUrl
                    posterName: messageFromUser.userName
                    postDate: Qt.formatDateTime(messageCreateDate)
                    onClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                            { userId: messageFromUser.id })
                }
                Label {
                    text: messageSubject
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.bold: !messageRead
                    textFormat: Text.StyledText
                    linkColor: Theme.highlightColor
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }

            menu: ContextMenu {
                enabled: !messageRead
                MenuItem {
                    visible: !messageRead
                    text: qsTr("Mark as read")
                    onClicked: sailreadsManager.markMessageAsRead(messageId)
                }
            }

            Separator {
                id: separator
                anchors {
                    top: column.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            onClicked: pageStack.push(Qt.resolvedUrl("MessagePage.qml"),
                    { message: messageMessage, messageId: messageId })
        }

        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
