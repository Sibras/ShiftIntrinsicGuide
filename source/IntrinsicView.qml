import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0

ListView {
    id: intrinsicView
    anchors.fill: parent
    boundsBehavior: Flickable.StopAtBounds
    model: intrinsicProxyModel
    clip: true

    delegate: ItemDelegate {
        width: intrinsicView.width - 25
        rightPadding: 6
        bottomPadding: 6
        leftPadding: 6
        topPadding: 6
        onClicked: state == 'collapsed' ? state = "" : state = 'collapsed';
        contentItem: ColumnLayout {
            spacing: 0
            Rectangle {
                height: 1
                Layout.preferredWidth: parent.width
                Layout.minimumWidth: Layout.preferredWidth
                color: catColours[intrinsicTechnology]
            }
            RowLayout {
                spacing: 0
                Rectangle {
                    color: catColours[intrinsicTechnology]
                    width: 15
                    height: 20
                }
                Label {
                    id: intrinName
                    text: intrinsicFullName
                    leftPadding: 5
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                }
                Label {
                    text: intrinsicInstruction
                    rightPadding: 5
                    topPadding: 0
                    bottomPadding: 0
                    font.pointSize: Qt.application.font.pixelSize - 2
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight
                }
            }
            ColumnLayout {
                spacing: 0
                id: intrinDetails
                Label {
                    text: "<h3>Synopsis</h3>"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    topPadding: 6
                    bottomPadding: -15
                }
                Label {
                    text: "Intrinsic: " + intrinsicName
                    leftPadding: 30
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    font.pointSize: Qt.application.font.pixelSize - 2
                }
                Label {
                    text: "Header: <" + intrinsicHeader + ">"
                    leftPadding: 30
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    font.pointSize: Qt.application.font.pixelSize - 2
                }
                Label {
                    text: "<h3>Description</h3>"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    topPadding: 6
                    bottomPadding: -15
                }
                Label {
                    text: intrinsicDescription
                    leftPadding: 30
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    font.pointSize: Qt.application.font.pixelSize - 2
                }
                Label {
                    text: "<h3>Operation</h3>"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 20
                    topPadding: 6
                    bottomPadding: -15
                    visible: intrinsicOperation.length !== 0
                }
                Label {
                    text: intrinsicOperation
                    leftPadding: 30
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    font.pointSize: Qt.application.font.pixelSize - 2
                    visible: intrinsicOperation.length !== 0
                }
            }
        }
        states: [
            State {
                name: "collapsed"
                PropertyChanges {
                    target: intrinDetails
                    height: 0
                    opacity: 0
                    visible: false
                }
            }
        ]
        state: 'collapsed'
        transitions: [
            Transition {
                NumberAnimation { target: intrinDetails; property: "height"; duration: 200 }
                NumberAnimation { target: intrinDetails; property: "opacity"; duration: 200 }
            }
        ]
    }
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }
}
