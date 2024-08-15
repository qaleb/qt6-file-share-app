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

    // Function to extract the base name from the current folder
    function extractBaseName(folderPath) {
        if (folderPath) {
            var pathString = folderPath.toString(); // Ensure the path is a string
            var parts = pathString.split("/");
            return parts[parts.length - 1];
        }
        return "";
    }

    // Function to go up one folder level
    function navigateUpFolder() {
        if (folderModel.folder && folderModel.folder !== "/") {
            var newPath = folderModel.folder.toString().split("/");
            newPath.pop();
            folderModel.folder = newPath.join("/") || "/"; // Handle empty path
            dialogTitle.text = extractBaseName(folderModel.folder);
            currentPathText.text = folderModel.folder;
        }
    }

    // Title and Current Path Display
    SmoothText {
        id: dialogTitle
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            leftMargin: 15
            right: parent.right
            rightMargin: 15
        }
        font.pixelSize: 64
        text: extractBaseName(folderModel.folder) // Set the title to the base name
        color: theme.color3
    }

    Text {
        id: currentPathText
        anchors {
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
            right: parent.right
        }

        // Folder List View
        ListView {
            id: folderListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: FolderListModel {
                id: folderModel
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
                    dialogTitle.text = fileBaseName;
                    currentPathText.text = filePath;
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
        onAcceptFolder: {
            guiBehind.changeDestinationFolder(currentPathText.text);
            fileFolderDialog.back()
        }

        onGoUpFolder: {
            navigateUpFolder(); // Call the function to go up one folder level
        }
        visible: true
    }
}
