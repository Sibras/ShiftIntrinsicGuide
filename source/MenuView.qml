import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0
import ShiftIntrinsicGuide 1.0

Row {
    anchors.fill: parent
    leftPadding: 10
    rightPadding: 10
    topPadding: 10
    bottomPadding: 10

    Flickable {
        id: menuView
        width: parent.width - parent.leftPadding - parent.rightPadding
        height: parent.height - parent.topPadding - parent.bottomPadding
        flickableDirection: Flickable.VerticalFlick
        contentHeight: contentItem.childrenRect.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Column {
            spacing: 0
            Label {
                text: "<h3>Instruction Set</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.StyledText
                topPadding: 6
                bottomPadding: 5
            }

            Repeater {
                model: technologiesModel
                delegate: ItemDelegate {
                    id: techDelegate
                    width: menuView.width - leftPadding - rightPadding - 10
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    onClicked: { techCheck.toggle(); techCheck.toggled() }
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            id: techCheck
                            checkable: true
                            checked: technologyChecked
                            onToggled: technologiesModel.setData(technologiesModel.index(index, 0), checked, TechnologyModel.TechnologyRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                            background: Rectangle {
                                color: catColours[index >= 0 ? index : 0]
                                width: parent.height * 1.3
                                height: parent.height * 1.3
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                        Label {
                            text: technologyName
                            leftPadding: 5
                            topPadding: 0
                            bottomPadding: 0
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                        }

                    }
                }
            }

            Label {
                text: "<h3>Type</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.StyledText
                topPadding: 10
                bottomPadding: 5
            }
            Repeater {
                model: typesModel
                delegate: ItemDelegate {
                    id: typeDelegate
                    width: menuView.width - leftPadding - rightPadding - 10
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    onClicked: { typeCheck.toggle(); typeCheck.toggled() }
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            id: typeCheck
                            checkable: true
                            checked: typeChecked
                            onToggled: typesModel.setData(typesModel.index(index, 0), checked, TypeModel.TypeRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                        }
                        Label {
                            text: typeName
                            leftPadding: 5
                            topPadding: 0
                            bottomPadding: 0
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                        }

                    }
                }
            }

            Label {
                text: "<h3>Category</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                textFormat: Text.StyledText
                topPadding: 10
                bottomPadding: 5
            }
            Repeater {
                model: categoriesModel
                delegate: ItemDelegate {
                    id: catDelegate
                    width: menuView.width - leftPadding - rightPadding - 10
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    onClicked: { catCheck.toggle(); catCheck.toggled() }
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            id: catCheck
                            checkable: true
                            checked: categoryChecked
                            onToggled: categoriesModel.setData(categoriesModel.index(index, 0), checked, CategoryModel.CategoryRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                        }
                        Label {
                            text: categoryName
                            leftPadding: 5
                            topPadding: 0
                            bottomPadding: 0
                            Layout.fillWidth: true
                            verticalAlignment: Text.AlignVCenter
                            textFormat: Text.PlainText
                        }

                    }
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
        }
    }
}
