/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0

Item {
    id: mainPage
    width: parent.width
    height: parent.height

    property variant catColours: ["darkgrey", "firebrick", "tomato", "sandybrown", "gold", "greenyellow", 
        "forestgreen", "turquoise", "dodgerblue", "midnightblue", "slateblue", "darkviolet", 
        "mediumvioletred", "saddlebrown", "darkorange", "red", "white", "white", "white"]

    Pane {
        anchors.centerIn: parent
        visible: application.progress !== 1
        id: pane
        width: 680
        height: 300

        Label {
            id: label
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 140
            width: 500
            height: 50
            text: application.LoadingTitle
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
        }

        ProgressBar {
            id: progressBar
            anchors.centerIn: parent
            width: parent.width - 60
            height: 80
            value: application.progress
        }
    }

    Frame {
        id: intrinsicFrame
        visible: application.progress === 1
        width: parent.width * 0.7 - anchors.rightMargin * 1.5
        bottomPadding: 0
        rightPadding: 0
        leftPadding: 0
        topPadding: 0
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        TextField {
            id: intrinsicSearch
            width: parent.width - 25
            height: 35
            verticalAlignment: Text.AlignVCenter
            rightPadding: 6
            bottomPadding: 6
            leftPadding: 6
            topPadding: 6
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 17
            anchors.rightMargin: 50
            anchors.leftMargin: 50
            selectByMouse: true
            placeholderText: "Search"
            onEditingFinished: intrinsicProxyModel.setFilterExpression(text)
        }
        Pane {
            anchors.top: parent.top
            anchors.topMargin: intrinsicSearch.height + intrinsicSearch.anchors.topMargin
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.rightMargin: 3
            anchors.leftMargin: 5
            IntrinsicView {
                id: intrinsicView
                anchors.fill: parent
            }
        }
    }

    Frame {
        id: menuFrame
        visible: application.progress === 1
        width: parent.width * 0.3 - anchors.leftMargin * 1.5
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
        topPadding: 0
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        MenuView {
            id: menuView
            anchors.fill: parent
        }
    }

    BusyIndicator {
        id: busyIndicator
        width: 60
        height: 64
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 84
        anchors.horizontalCenterOffset: 0
        running: !application.isLoaded
    }
}
