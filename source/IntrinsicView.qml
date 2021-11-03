import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

ListView {
    id: intrinsicView
    anchors.fill: parent
    boundsBehavior: Flickable.StopAtBounds
    model: intrinsicsModel

    delegate: ItemDelegate {
        width: intrinsicView.width - 25
        rightPadding: 6
        bottomPadding: 6
        leftPadding: 6
        topPadding: 6
        contentItem: RowLayout {
            spacing: 0

        }
    }
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }
}
