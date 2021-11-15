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
        id: rootDelegate
        width: intrinsicView.width - 25
        rightPadding: 6
        bottomPadding: 6
        leftPadding: 6
        topPadding: 6
        onClicked: rootDelegate.state == 'collapsed' ? rootDelegate.state = "" : rootDelegate.state = 'collapsed';
        contentItem: ColumnLayout {
            spacing: 0
            id: intrinShort
            Rectangle {
                height: 1
                Layout.preferredWidth: parent.width
                Layout.minimumWidth: Layout.preferredWidth
                color: catColours[intrinsicTechnology]
            }
            RowLayout {
                spacing: 0
                Rectangle {
                    id: colourRect
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
                    id: intrinInstruction
                    text: intrinsicInstruction
                    rightPadding: 5
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight
                }
            }
            RowLayout {
                spacing: 0
                id: intrinDetails
                Label {
                    id: intrinDescription
                    text: intrinsicDescription
                    leftPadding: 5
                    topPadding: 0
                    bottomPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
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
