import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick 2.0

//window containing the application
ApplicationWindow {

    visible: true

    //title of the application
    title: qsTr("Hello World")
    width: 640
    height: 480

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: zoomMenu.open()
    }

    menuBar: MenuBar {
        id: zoomMenu

        Menu {
            title: "Choose algorithm"
            MenuItem {
                text: "Ohlander"
                onTriggered: olander
            }
        }

        MenuItem {
            text: qsTr("Zoom In")
//            shortcut: StandardKey.ZoomIn
            onTriggered: zoomIn()
        }

        MenuItem {
            text: qsTr("Zoom Out")
//            shortcut: StandardKey.ZoomOut
            onTriggered: zoomOut()
        }
    }

    //menu containing two menu items


        background: Rectangle {
            implicitHeight: 10
            color: "#e0e0e0"
        }
    }

    //Content Area

    //a button in the middle of the content area
//    Button {
//        text: qsTr("Hello World")
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
//    }
}
