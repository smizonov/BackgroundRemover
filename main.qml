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
                width: 200
                textRole: "name"
                model: ListModel {
                    id: varietiesMethods

                    ListElement {
                        name: "ML u2net"
                        method: RmBgMethod.MLU2net
                    }

                    ListElement {
                        name: "ML dis"
                        method: RmBgMethod.MLDis
                    }

                    ListElement {
                        name: "Ohlander algorithm"
                        method: RmBgMethod.Ohlander
                    }

                    ListElement {
                        name: "KMeans algorithm"
                        method: RmBgMethod.KMeans
                    }

                    ListElement {
                        name: "Subtruction algorithm"
                        method: RmBgMethod.Extruction
                    }
                }

                onCurrentIndexChanged: {
                    viewModel.method = varietiesMethods.get(currentIndex).method
                }
            }

            Row {
                Button {
                    text: "Select source folder"
                    onClicked: {
                        viewModel.srcFolder = viewModel.showDirectoryDialog(
                                    StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0],
                                    "Select source folder")
                    }
                }

                Label {
                    text: viewModel.srcFolder
                }
            }

            Row {
                Button {
                    text: "Select destination folder"
                    onClicked: {
                        viewModel.dstFolder = viewModel.showDirectoryDialog(
                                    StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0],
                                    "Select destination folder")
                    }
                }

                Label {
                    text: viewModel.dstFolder
                }
            }

            Row {
                Button {
                    width: 350
                    text: "Select background image (for extruction algorithm)"
                    onClicked: {
                        viewModel.bgImagePath = viewModel.showOpenDialog(
                                    StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0],
                                    "Select background image")
                    }
                    enabled: viewModel.method === RmBgMethod.Extruction
                }

                Label {
                    text: viewModel.bgImagePath
                }
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
        }
    }

    ProgressBar {
        anchors.bottom: parent.bottom
        value: viewModel.progress
    }
}
