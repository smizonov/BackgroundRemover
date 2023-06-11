import QtQuick 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: previewImages
    property var viewModel: null

    width: 100
    height: 200

    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: 10

        Text {
            text: "Source image"
        }

        Text {
            text: "Image without background"
        }

        Image {
            id: srcImage
            source: viewModel ? viewModel.srcImagePath : ""
            width: 10
            height: 20
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }

        Image {
            id: dstImage
            source: viewModel ? viewModel.dstImagePath : ""
            width: 10
            height: 20
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }

        // Second row
        Button {
            text: "Continue"
            width: srcImage / 2
//            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            onClicked: {
                if (!viewModel)
                    return

                viewModel.continueSelected()
            }
        }

        Button {
            text: "Choose Another Algorithm"
            width: dstImage / 2
//            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            onClicked: {
                if (!viewModel)
                    return

                viewModel.stopSelected()
            }
        }
    }
}
