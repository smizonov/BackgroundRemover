import QtQuick 2.15
import QtQuick.Dialogs 1.3

FileDialog {
    id: fileDialog
    folder: true

    signal folderSelected(string folder)

    onAccepted: folderSelected(fileDialog.folder)
    onRejected: console.log("Folder selection canceled")
}