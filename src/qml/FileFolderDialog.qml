import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel
import QtQuick.Layouts

Rectangle {
    id: fileFolderDialog
    color: theme.color6
    focus: true
    anchors.fill: parent

    property alias folder: folderModel.folder

    signal back();

    // Title and Current Path Display
    SmoothText {
        id: dialogTitle
        anchors{
            top: parent.top
            topMargin: 5
            left: parent.left
            leftMargin: 15
            right: parent.right
            rightMargin: 15
        }
        font.pixelSize: 64
        text: folderModel.folder
        color: theme.color3
    }

    Text {
        id: currentPathText
        anchors{
            top: dialogTitle.bottom
            topMargin: -35
            left: parent.left
            leftMargin: 15
            right: parent.right
            rightMargin: 15
        }
        text: folderModel.folder
        font.pixelSize: 16
        color: theme.color1
    }

    ColumnLayout {
        anchors {
            top: currentPathText.bottom
            topMargin: 10
            bottom: parent.bottom
            bottomMargin: 75
            left: parent.left
            // leftMargin: 10
            right: parent.right
            // rightMargin: 10
        }

        // spacing: 10

        // Folder List View
        ListView {
            id: folderListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: FolderListModel {
                id: folderModel
                // folder: "/storage/emulated/0"
                folder: guiBehind.currentPath
                nameFilters: ["*.*"]
                showFiles: false
                showOnlyReadable: true
            }

            delegate: ItemDelegate {
                width: folderListView.width
                text: fileName
                font.pixelSize: 18
                icon.source: folderModel.isFolder(index) ? "qrc:/assets/icons/folder-fill.svg" : "qrc:/assets/icons/file-fill.svg"
                onClicked: {
                    // console.log("Currently navigated path is: " + fileUrl);
                    // console.log("parentFolder: " + folderModel.parentFolder);
                    // console.log("rootFolder : " + folderModel.rootFolder );
                    // console.log("filePath : " + filePath );

                    dialogTitle.text = fileBaseName;
                    currentPathText.text = fileUrl;
                    if (folderModel.isFolder(index)) {
                        folderModel.folder += "/" + fileName
                    } else {
                        fileFolderDialog.selectedFolderUrl = folderModel.folder + "/" + fileName
                    }
                }
            }
        }
    }

    DialogNavBar {
        id: bottomToolBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        onBack: fileFolderDialog.back()
        visible: true
    }
}
