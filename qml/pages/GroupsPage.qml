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
    id: groupsPage

    property string userId: ""
    property bool busy: sailreadsManager.busy && groupsPage.status === PageStatus.Active

    Component.onDestruction: {
        groupsModel.cancelRequest()
    }

    UserGroupsModel {
        id: groupsModel
        userId: groupsPage.userId
    }

    SilicaListView {
        id: groupsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Groups")
        }

        PullDownMenu {
            busy: groupsPage.busy
            MenuItem {
                text: qsTr("Search")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/GroupSearchPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    groupsModel.loadGroups()
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && groupsView.count === 0
            text: qsTr ("There are no groups")
            hintText: qsTr("Pull down to refresh")
        }

        cacheBuffer: groupsPage.height
        model: groupsModel

        function fetchMoreIfNeeded() {
            if (!groupsPage.busy &&
                    groupsModel.hasMore &&
                    indexAt(contentX, contentY + height) > groupsModel.rowCount() - 2) {
                groupsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

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

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupsPage.busy
        visible: running
    }
}
