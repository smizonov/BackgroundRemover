import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import QtQuick.Controls 1.4
import QtQuick 2.0
import Qt.labs.platform 1.1
import bgRemover 1.0

//window containing the application
ApplicationWindow {

    function openFolderDialog(button) {
        var dialog = Qt.createQmlObject(
            'import QtQuick.Dialogs 1.3; FileDialog { folder: true }',
            button
        )

        dialog.accepted.connect(function() {
            button.folderPath = dialog.folder
            console.log("Selected folder path:", button.folderPath)
            dialog.destroy() // Clean up the dialog
        })

        dialog.rejected.connect(function() {
            console.log("Folder selection canceled")
            dialog.destroy() // Clean up the dialog
        })

        dialog.open()
    }

    visible: true
//    property var sortModeModel: []

//    readonly property var interface : dataContext

    //title of the application
    title: qsTr("Background remover")
    width: 640
    height: 480

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: zoomMenu.open()
    }

    Item {
        anchors.fill: parent

        Column {
            // Add your desired layout properties

            ComboBox {
                width: 150
                textRole: "name"
                model: ListModel {
                    id: varietiesMethods
                    ListElement {
                        name: "Ohlander algorithm"
                        method: RmBgMethod.Ohlander
                    }

                    ListElement {
                        name: "KMeans algorithm"
                        method: RmBgMethod.KMeans
                    }

                    ListElement {
                        name: "Substruction algorithm"
                        method: RmBgMethod.Substruction
                    }
                }

                onCurrentIndexChanged: {
                    console.log(varietiesMethods.get(currentIndex).method)
                    console.log(RmBgMethod.Substruction)
                    viewModel.method = varietiesMethods.get(currentIndex).method
                }
            }

            Row {
                Button {
                    text: "Select src folder"
                    onClicked: {
                        folderDialog.open()
                        folderDialog.title = "Select src folder"
                        folderDialog.dialogType = "srcFolderSelectionButton"
                    }
                }


                Label {
                    text: viewModel.srcFolder
                }
            }



            Button {
                text: "Select dst folder"
                onClicked: {
                    folderDialog.open()
                    folderDialog.title = "Select dst folder"
                    folderDialog.dialogType = "dstFolderSelectionButton"
                }
            }

            Button {
                text: "Select background image"
                onClicked: {
                    fileDialog.open()
                    fileDialog.title = "Select background image"
                }
                enabled: viewModel.method === RmBgMethod.Substruction
            }

            Button {
                text: "Start!"
                onClicked: {
                    viewModel.start()
                }
            }
            Button {
                text: "Stop"
                onClicked: {
                    viewModel.stop()
                }
            }

            FileDialog {
                id: fileDialog
                folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                onAccepted: {
                    var dstPath = folderDialog.folder.toString().slice(8)
                    viewModel.bgImagePath = dstPath
                }
            }

            FolderDialog {
                id: folderDialog
                folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                visible: false

                property string dialogType: "" // Custom property to store the button type

                onAccepted: {
                    var dstFolder = folderDialog.folder.toString().slice(8)
                    if (folderDialog.dialogType === "srcFolderSelectionButton")
                        viewModel.srcFolder = dstFolder
                    else if (folderDialog.dialogType === "dstFolderSelectionButton")
                        viewModel.dstFolder = dstFolder
                }
            }
        }
    }

    ProgressBar {
        anchors.bottom: parent.bottom
        value: viewModel.progress
    }
}
