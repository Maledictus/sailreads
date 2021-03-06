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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0
import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: groupSearchPage
    property bool busy: sailreadsManager.busy && groupSearchPage.status === PageStatus.Active

    Component.onDestruction: {
        groupsModel.cancelRequest()
    }

    FoundGroupsModel {
        id: groupsModel
    }

    SilicaListView {
        id: groupsView

        anchors.fill: parent

        cacheBuffer: groupSearchPage.height

        model: groupsModel

        function fetchMoreIfNeeded() {
            if (!groupSearchPage.busy &&
                    groupsModel.hasMore &&
                    indexAt(contentX, contentY + height) > groupsModel.rowCount() - 2) {
                groupsModel.fetchMoreContent(searchEntry.text)
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        header: Item {
            width: groupsView.width
            height: headerBox.height
        }

        delegate: GroupListItem {
            width: groupsView.width

            imageUrl: groupImageUrl
            title: groupName
            lastActivity: groupLastActivity
            usersCount: groupUsersCount

            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/GroupPage.qml"),
                        { groupId: groupId, groupName: groupName })
            }
        }

        VerticalScrollDecorator {}
    }

    Column {
        id: headerBox
        parent: groupsView.headerItem ? groupsView.headerItem : groupSearchPage
        width: parent.width

        PageHeader {
            id: pageHeader
            title: qsTr("Search group")
        }

        SearchField {
            id: searchEntry

            function search(searchTerm) {
                focus = false
                if (text !== searchTerm) {
                    text = searchTerm
                }

                groupsModel.fetchMoreContent(searchTerm);
            }

            width: parent.width
            placeholderText: qsTr("Search")

            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.enabled: text != ""
            EnterKey.onClicked: {
                if (text !== "") {
                    focus = false
                    search(text)
                }
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupSearchPage.busy
        visible: running
    }
}
