import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

Item {
    property string folderPath: ""
    signal folderSelected

    Row {
        anchors.fill: parent

        Button {
            text: parent.text
            onClicked: openFolderDialog()
        }
    }

    function openFolderDialog() {
           var dialog = Dialog {
               title: "Select Folder"
               modal: true
               standardButtons: Dialog.Ok | Dialog.Cancel
               contentItem: FileDialog {
                   id: fileDialog
                   folder: true

                   onAccepted: {
                       parent.folderPath = fileDialog.folder
                       console.log("Selected folder path:", parent.folderPath)
                       parent.folderSelected() // Emit the custom signal
                       dialog.close()
                   }

                   onRejected: {
                       console.log("Folder selection canceled")
                       dialog.close()
                   }
               }
           }

           dialog.open()

    function openFolderDialog() {
        var dialog = FileDialog {
            folder: true

            onAccepted: {
                parent.folderPath = dialog.folder
                console.log("Selected folder path:", parent.folderPath)
                parent.folderSelected() // Emit the custom signal
            }

            onRejected: {
                console.log("Folder selection canceled")
            }
        }

        dialog.open()
    }
}
