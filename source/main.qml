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

import QtQuick.Controls.Universal
import QtQuick
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 1920
    height: 1080
    title: "Shift Intrinsic Guide"
    Universal.theme: Universal.Dark

    Item {
        anchors.centerIn: parent
        width: infoDialog.width
        height: infoDialog.height
        Dialog {
            id: infoDialog
            title: application.OKDialogTitle
            visible: application.hasOKDialog

            standardButtons: Dialog.Ok

            onAccepted: application.acceptOKCallback()
        }
    }

    Page1 {
    }
}
