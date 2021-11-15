import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0
import ShiftIntrinsicGuide 1.0

Row {
    id: menuView
    anchors.fill: parent
    leftPadding: 6
    
    Flickable {
        width: parent.width
        height: parent.height
        flickableDirection: Flickable.VerticalFlick
        contentHeight: contentItem.childrenRect.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Column {
            Label {
                id: technologylabel
                text: "Instruction Set"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pointSize: 16
                topPadding: 6
                bottomPadding: 6
            }

            Repeater {
                model: technologiesModel
                delegate: ItemDelegate {
                    width: menuView.width - 25
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            checkable: true
                            checked: technologyChecked
                            onToggled: technologiesModel.setData(technologiesModel.index(index, 0), checked, TechnologyModel.TechnologyRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                            background: Rectangle {
                                color: catColours[index]
                                radius: 25
                            }
                        }
                        Label {
                            text: technologyName
                            leftPadding: 10
                            topPadding: 0
                            bottomPadding: 0
                            Layout.preferredWidth: menuView.width - 25 - 10 - 6
                            Layout.minimumWidth: Layout.preferredWidth
                            verticalAlignment: Text.AlignVCenter
                        }

                    }
                }
            }

            Label {
                id: typelabel
                text: "Type"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pointSize: 16
                topPadding: 10
                bottomPadding: 6
            }
            Repeater {
                model: typesModel
                delegate: ItemDelegate {
                    width: menuView.width - 25
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            checkable: true
                            checked: typeChecked
                            onToggled: typesModel.setData(typesModel.index(index, 0), checked, TypeModel.TypeRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                        }
                        Label {
                            text: typeName
                            leftPadding: 10
                            topPadding: 0
                            bottomPadding: 0
                            Layout.preferredWidth: menuView.width - 25 - 10 - 6
                            Layout.minimumWidth: Layout.preferredWidth
                            verticalAlignment: Text.AlignVCenter
                        }

                    }
                }
            }

            Label {
                id: categorylabel
                text: "Category"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pointSize: 16
                topPadding: 10
                bottomPadding: 6
            }
            Repeater {
                model: categoriesModel
                delegate: ItemDelegate {
                    width: menuView.width - 25
                    rightPadding: 6
                    bottomPadding: 0
                    leftPadding: 6
                    topPadding: 0
                    contentItem: RowLayout {
                        spacing: 0
                        CheckBox {
                            checkable: true
                            checked: categoryChecked
                            onToggled: categoriesModel.setData(categoriesModel.index(index, 0), checked, CategoryModel.CategoryRoleChecked)
                            scale: 0.7
                            topPadding: 0
                            bottomPadding: 0
                        }
                        Label {
                            text: categoryName
                            leftPadding: 10
                            topPadding: 0
                            bottomPadding: 0
                            Layout.preferredWidth: menuView.width - 25 - 10 - 6
                            Layout.minimumWidth: Layout.preferredWidth
                            verticalAlignment: Text.AlignVCenter
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
