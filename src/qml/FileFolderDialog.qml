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
    property alias acceptIconVisible: bottomToolBar.acceptIconVisible
    property alias acceptTextVisible: bottomToolBar.acceptTextVisible
    property alias showFiles: folderModel.showFiles

    signal back();

    // Function to construct the path for the currentPathText
    function constructCurrentPath(folderPath) {
        if (folderPath) {
            var pathString = folderPath.toString();
            var baseIndex = pathString.indexOf("/storage/emulated/0");
            if (baseIndex !== -1) {
                var path = pathString.substring(baseIndex + "/storage/emulated/0".length);

                if(path === "") {
                    return "/";
                }

                return pathString.substring(baseIndex + "/storage/emulated/0".length);
            }
        }
        return "/";
    }

    // Function to go up one folder level
    function navigateUpFolder() {
        if (folderModel.folder && folderModel.folder !== "file:///storage/emulated/0") {
            var newPath = folderModel.folder.toString().split("/");
            if (newPath[newPath.length - 1] === "0") {
                return ; // Already at the root folder
            }
            newPath.pop();
            var newFolder = newPath.join("/") || "file:///storage/emulated/0"; // Handle empty path

            folderModel.folder = newFolder;
            currentPathText.text = constructCurrentPath(folderModel.folder);
        }
        return "/";
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
        // elide: "ElideMiddle"
        font.pixelSize: 64
        // text: extractBaseName(folderModel.folder) // Set the title to the base name
        text: "Select a Folder"
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
        elide: "ElideMiddle"
        text: constructCurrentPath(folderModel.folder)
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
                icon.source: {
                    if(folderModel.isFolder(index)){
                        "qrc:/assets/icons/folder-fill-colored.svg"
                    } else {
                        "qrc:/assets/icons/file-fill-colored.svg"
                    }
                }
                icon.cache: true
                icon.color: {
                    if(folderModel.isFolder(index)){
                        "#FFBE1E"
                    } else {
                        "#EEEEEE"
                    }
                }
                onClicked: {
                    if (folderModel.isFolder(index)) {
                        folderModel.folder += "/" + fileName;
                        currentPathText.text = constructCurrentPath(folderModel.folder);
                    } else {
                        fileFolderDialog.selectedFolderUrl = folderModel.folder + "/" + fileName;
                    }
                }
            }
        }
    }

    DialogNavBar {
        id: bottomToolBar
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        onBack: fileFolderDialog.back()
        onAcceptFolder: {
            guiBehind.changeDestinationFolder(folderModel.folder);
            fileFolderDialog.back()
        }

        onGoUpFolder: {
            navigateUpFolder(); // Call the function to go up one folder level
        }
        acceptIconVisible: true
        acceptTextVisible: true
    }
}
