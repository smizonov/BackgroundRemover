import QtQuick 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1 as Platform
import QtQuick.Controls 1.4 as OldControls
import bgRemover 1.0

Item {
//    id: previewImages
    required property var viewModel

    width: 800
    height: 400

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
            Layout.preferredHeight: 200
            Layout.preferredWidth: 100
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }

        Image {
            id: dstImage
            source: viewModel ? viewModel.dstImagePath : ""
            Layout.preferredHeight: 200
            Layout.preferredWidth: 100
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }

        // Second row
        OldControls.Button {
            text: "Continue"
            width: srcImage / 2
//            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                if (!viewModel)
                    return

                viewModel.continueSelected()
            }
        }

        OldControls.Button {
            text: "Choose Another Algorithm"

            width: dstImage / 2
//            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                console.log(viewModel)
                console.log(viewModel.srcImagePath)
                if (!viewModel)
                    return

                viewModel.stopSelected()
            }
        }
    }
}
