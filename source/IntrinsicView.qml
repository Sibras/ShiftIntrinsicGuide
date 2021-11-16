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
            RowLayout {
                spacing: 0
                id: intrinDetails
                Rectangle {
                    width: 1
                    Layout.preferredHeight: parent.height
                    Layout.minimumHeight: Layout.preferredHeight
                    color: catColours[intrinsicTechnology]
                }
                ColumnLayout {
                    spacing: 0
                    Label {
                        text: "<h3>Synopsis</h3>"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 20
                        topPadding: 6
                        bottomPadding: -15
                    }
                    Row {
                        Label {
                            id: intrinNameLabel
                            text: "Intrinsic: "
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                        TextEdit {
                            text: intrinsicName
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: Qt.application.font.pixelSize - 2
                            readOnly: true
                            selectByMouse: true
                            color: intrinNameLabel.color
                        }
                    }
                    Row {
                        Label {
                            text: "Header: <"
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                        TextEdit {
                            text: intrinsicHeader
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: Qt.application.font.pixelSize - 2
                            readOnly: true
                            selectByMouse: true
                            color: intrinNameLabel.color
                        }
                        Label {
                            text: ">"
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                    }
                    Label {
                        text: "CPUIDs: " + intrinsicCPUIDText
                        leftPadding: 30
                        topPadding: 0
                        bottomPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: Qt.application.font.pixelSize - 2
                        visible: intrinsicCPUIDText.length !== 0
                    }
                    Label {
                        text: "Types: " + intrinsicTypesText
                        leftPadding: 30
                        topPadding: 0
                        bottomPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: Qt.application.font.pixelSize - 2
                    }
                    Label {
                        text: "Categories: " + intrinsicCategoriesText
                        leftPadding: 30
                        topPadding: 0
                        bottomPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: Qt.application.font.pixelSize - 2
                    }
                    Label {
                        text: "<h3>Description</h3>"
                        verticalAlignment: Text.AlignVCenter
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
                    Label {
                        text: "<h3>Performance</h3>"
                        verticalAlignment: Text.AlignVCenter
                        bottomPadding: 0
                        leftPadding: 20
                        topPadding: 6
                    }
                    Item {
                        Layout.fillWidth: true                        
                        Layout.preferredHeight: 20 + verticalHeader.contentHeight + horizontalHeader.implicitHeight
                        Layout.minimumHeight: Layout.preferredHeight
                        HorizontalHeaderView {
                            id: horizontalHeader
                            syncView: tableView
                            anchors.left: tableView.left
                        }
                        VerticalHeaderView {
                            id: verticalHeader
                            syncView: tableView
                            anchors.right: tableView.left
                            anchors.top: tableView.top
                            leftMargin: 75
                        }
                        TableView {
                            id: tableView
                            interactive: false
                            anchors.fill: parent
                            topMargin: horizontalHeader.implicitHeight
                            leftMargin: 75
                            columnWidthProvider: function (column) { return 100; }
                            model: intrinsicMeasurements
                            clip: true
                            delegate: Label {
                                text: display
                                font.pixelSize: Qt.application.font.pixelSize
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                bottomPadding: 6
                                topPadding: 6
                                background: Rectangle{
                                    color: palette.shadow
                                }
                            }
                        }
                    }
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
