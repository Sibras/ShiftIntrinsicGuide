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
 
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: mainPage
    width: 1920
    height: 1080

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
