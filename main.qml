import QtQuick.Controls 2.4
import QtQuick.Window 2.15
//import QtQuick.Dialogs 1.2
//import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQml.Models 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as OldControls
import QtQuick 2.0
import Qt.labs.platform 1.1 as Platform
import bgRemover 1.0
import utils 1.0

//window containing the application
ApplicationWindow {
    visible: true
    id: view

    readonly property var previewImagesPtr: viewModel ? viewModel.previewImages : null

    //title of the application
    title: qsTr("Background remover")
    width: 640
    height: 500

    function previewRequested() {
        loader.active = true
    }

    function previewCancelRequested() {
        loader.active = false
    }

    Component.onCompleted: {
        viewModel.previewRequested.connect(previewRequested)
        viewModel.previewCancelRequested.connect(previewCancelRequested)
    }

    Platform.MenuBar {

        id: myMenuBar
        Platform.Menu {
            title: "File"
            Platform.MenuItem {
                text: "Close"
                onTriggered: Qt.callLater(Qt.quit)
            }
        }

        Platform.Menu {
            title: "Edit"
            Platform.MenuItem {
                text: "Select source folder"
                onTriggered: {
                    viewModel.srcFolder = Utils.showDirectoryDialog(
                                Platform.StandardPaths.standardLocations(Platform.StandardPaths.PicturesLocation)[0],
                                "Select source folder")
                }

            }
            Platform.MenuItem {
                text: "Select destination folder"
                onTriggered: {
                    viewModel.dstFolder = Utils.showDirectoryDialog(
                                Platform.StandardPaths.standardLocations(Platform.StandardPaths.PicturesLocation)[0],
                                "Select destination folder")
                }
            }
            Platform.MenuItem {
                text: "Select background image"
                enabled: viewModel.method === RmBgMethod.Extruction
                onTriggered: {
                    viewModel.bgImagePath = Utils.showOpenDialog(
                                Platform.StandardPaths.standardLocations(Platform.StandardPaths.PicturesLocation)[0],
                                "Select background image")
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: zoomMenu.open()
    }

    ColumnLayout {

        spacing: 3
        anchors.fill: parent

        Row {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
            OldControls.ComboBox {
                id: methods
                width: 200
                textRole: "name"

                style: ComboBoxStyle {
                    background: systemPalette.menu
                }
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

            OldControls.Button {
                id: startBotton
                text: "Start!"
                enabled: viewModel.srcFolder !== "" && viewModel.dstFolder !== ""
                onClicked: {
                    viewModel.start()
                }
            }
            OldControls.Button {
                id: stopBotton
                text: "Stop"
                enabled: startBotton.enabled
                onClicked: {
                    viewModel.stop()
                }
            }
        }

        Loader {
            id: loader
            //            visible: false
            //            Layout.alignment: Qt.AlignHBottom | Qt.AlignVCenter
            Layout.fillWidth: true
            sourceComponent: previewImagesComponent
            width: 800
            height: 400
            active: false

            Component.onCompleted: {
                console.log("loader created")
            }
        }

        OldControls.Button {
            id: showDstFolder
            text: "Show images without background"
            //            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                Utils.showInExplorer(viewModel.dstFolder)
            }
        }

    Component {
        id: previewImagesComponent
        PreviewImages {
            viewModel: previewImagesPtr
        }
    }
    Rectangle {

        //        color: "lightgray"
//        color: "transparent"
//        border.color: "black"
//        border.width: 2
        width: 300
        height: 30
        Layout.bottomMargin: 15
        Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

        ProgressBar {
            id: progressBar

            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            value: viewModel.processedCount / viewModel.totalCount
            height: 15
            background: Rectangle {
                anchors.fill: progressBar
                color: "lightgray"
//                radius: 4
            }

            contentItem: Rectangle {
                anchors.left: progressBar.left
                anchors.bottom: progressBar.bottom
                height: progressBar.height
                width: progressBar.value === 0 ? progressBar.width : progressBar.width * progressBar.value
                color: "lightgreen"
//                radius: 4
            }
            //            anchors.fill: parent
        }
        Text {
            anchors.centerIn: parent
            text: viewModel.processedCount + "/" + viewModel.totalCount + " processed"
        }
    }
}

}
