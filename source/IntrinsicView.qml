import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0

ListView {
    id: intrinsicView
    anchors.fill: parent
    boundsBehavior: Flickable.StopAtBounds
    model: intrinsicProxyModel
    clip: true
    reuseItems: true

    delegate: ItemDelegate {
        width: intrinsicView.width - 25
        rightPadding: 6
        bottomPadding: 6
        leftPadding: 6
        topPadding: 6
        onClicked: {
            state == 'collapsed' ? state = 'expanded' : state = 'collapsed';
            state == 'collapsed' ? intrinsicExpanded = false : intrinsicExpanded = true;
        }
        ListView.onPooled: intrinsicExpanded ? state = 'expanded' : state = 'collapsed';
        ListView.onReused: intrinsicExpanded ? state = 'expanded' : state = 'collapsed';
        contentItem: ColumnLayout {
            id: intrinsicItem
            spacing: 0
            Layout.fillWidth: true
            Rectangle {
                height: 1
                Layout.preferredWidth: parent.width
                Layout.minimumWidth: Layout.preferredWidth
                color: catColours[intrinsicTechnology]
            }
            RowLayout {
                spacing: 0
                Layout.preferredWidth: parent.width
                Layout.minimumWidth: Layout.preferredWidth
                Layout.maximumWidth: Layout.preferredWidth
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
                    textFormat: Text.StyledText
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
                    Layout.maximumWidth: intrinsicItem.width / 4
                    elide: Text.ElideRight
                    textFormat: Text.PlainText
                }
            }
            Component {
                id: intrinComponent
                RowLayout {
                    spacing: 0
                    id: intrinDetails
                    Layout.preferredHeight: intrinDetailsSub.implicitHeight
                    Layout.maximumHeight: Layout.preferredHeight
                    width: intrinsicItem.width
                    clip: true
                    Rectangle {
                        width: 1
                        Layout.preferredHeight: parent.height
                        Layout.minimumHeight: Layout.preferredHeight
                        color: catColours[intrinsicTechnology]
                    }
                    ColumnLayout {
                        id: intrinDetailsSub
                        spacing: 0
                        Label {
                            text: "<h3>Synopsis</h3>"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            textFormat: Text.StyledText
                            leftPadding: 20
                            topPadding: 6
                            bottomPadding: 0
                        }
                        Row {
                            Label {
                                id: intrinNameLabel
                                text: "Intrinsic: "
                                leftPadding: 30
                                topPadding: 0
                                bottomPadding: 0
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.PlainText
                                font.pointSize: Qt.application.font.pixelSize - 2
                            }
                            TextEdit {
                                text: intrinsicName
                                topPadding: 0
                                bottomPadding: 0
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.PlainText
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
                                textFormat: Text.PlainText
                                font.pointSize: Qt.application.font.pixelSize - 2
                            }
                            TextEdit {
                                text: intrinsicHeader
                                topPadding: 0
                                bottomPadding: 0
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.PlainText
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
                                textFormat: Text.PlainText
                                font.pointSize: Qt.application.font.pixelSize - 2
                            }
                        }
                        Row {
                            Label {
                                text: "Instruction: "
                                leftPadding: 30
                                topPadding: 0
                                bottomPadding: 0
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.PlainText
                                font.pointSize: Qt.application.font.pixelSize - 2
                            }
                            TextEdit {
                                text: intrinsicInstruction
                                topPadding: 0
                                bottomPadding: 0
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.PlainText
                                font.pointSize: Qt.application.font.pixelSize - 2
                                readOnly: true
                                selectByMouse: true
                                color: intrinNameLabel.color
                            }
                        }
                        Label {
                            text: "CPUIDs: " + intrinsicCPUIDText
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                            font.pointSize: Qt.application.font.pixelSize - 2
                            visible: intrinsicCPUIDText.length !== 0
                        }
                        Label {
                            text: "Types: " + intrinsicTypesText
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                        Label {
                            text: "Categories: " + intrinsicCategoriesText
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                        Label {
                            text: "<h3>Description</h3>"
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.StyledText
                            leftPadding: 20
                            topPadding: 6
                            bottomPadding: 0
                        }
                        Label {
                            text: intrinsicDescription
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.StyledText
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            font.pointSize: Qt.application.font.pixelSize - 2
                        }
                        Label {
                            text: "<h3>Operation</h3>"
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.StyledText
                            leftPadding: 20
                            topPadding: 6
                            bottomPadding: 0
                            visible: intrinsicOperation.length !== 0
                        }
                        Label {
                            text: intrinsicOperation
                            leftPadding: 30
                            topPadding: 0
                            bottomPadding: 0
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            font.pointSize: Qt.application.font.pixelSize - 2
                            visible: intrinsicOperation.length !== 0
                        }
                        ColumnLayout {
                            spacing: 0
                            visible: intrinsicMeasurements.rowCount() > 0
                            Label {
                                text: "<h3>Performance</h3>"
                                verticalAlignment: Text.AlignVCenter
                                textFormat: Text.StyledText
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
                                    leftMargin: 180
                                    width: 150
                                }
                                TableView {
                                    id: tableView
                                    interactive: false
                                    anchors.fill: parent
                                    topMargin: horizontalHeader.implicitHeight
                                    leftMargin: verticalHeader.leftMargin
                                    columnWidthProvider: function (column) { return column < 3 ? 85 : 240; }
                                    model: intrinsicMeasurements
                                    clip: true
                                    delegate: Label {
                                        text: display
                                        font.pixelSize: Qt.application.font.pixelSize
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        textFormat: Text.PlainText
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
            }
            Loader {
                id: detailsLoader
            }
        }
        states: [
            State {
                name: "collapsed"
                PropertyChanges {
                    target: detailsLoader
                    visible: false
                    sourceComponent: undefined
                }
            }, 
            State {
                name: "expanded"
                PropertyChanges {
                    target: detailsLoader
                    visible: true
                    sourceComponent: intrinComponent
                }
            }
        ]
        state: 'collapsed'
    }
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }
}
