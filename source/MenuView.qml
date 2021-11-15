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
                text: "<h3>Instruction Set</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                topPadding: 6
                bottomPadding: -15
            }

            Repeater {
                model: technologiesModel
                delegate: ItemDelegate {
                    id: techDelegate
                    width: menuView.width - 25
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
                                color: catColours[index]
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
                            Layout.preferredWidth: techDelegate.width - techDelegate.rightPadding - leftPadding
                            Layout.minimumWidth: Layout.preferredWidth
                            verticalAlignment: Text.AlignVCenter
                        }

                    }
                }
            }

            Label {
                text: "<h3>Type</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                topPadding: 10
                bottomPadding: -15
            }
            Repeater {
                model: typesModel
                delegate: ItemDelegate {
                    id: typeDelegate
                    width: menuView.width - 25
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
                            Layout.preferredWidth: typeDelegate.width - typeDelegate.rightPadding - leftPadding
                            Layout.minimumWidth: Layout.preferredWidth
                            verticalAlignment: Text.AlignVCenter
                        }

                    }
                }
            }

            Label {
                text: "<h3>Category</h3>"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                topPadding: 10
                bottomPadding: -15
            }
            Repeater {
                model: categoriesModel
                delegate: ItemDelegate {
                    id: catDelegate
                    width: menuView.width - 25
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
                            Layout.preferredWidth: catDelegate.width - catDelegate.rightPadding - leftPadding
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
