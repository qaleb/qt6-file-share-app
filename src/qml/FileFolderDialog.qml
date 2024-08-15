import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel
import QtQuick.Layouts

Dialog {
    id: fileFolderDialog
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    implicitWidth: 500
    implicitHeight: 400
    property alias folder: folderModel.folder
    property alias nameFilters: folderModel.nameFilters
    property alias showFiles: folderModel.showFiles
    property url fileUrl
    standardButtons: Dialog.Ok | Dialog.Cancel
    ListView {
        id: fileFolderView
        anchors.fill: parent
        clip: true
        currentIndex: -1
        model: FolderListModel {
            id: folderModel
            folder: "file:/tmp"
            nameFilters: ["*.*"]
            showFiles: false
        }
        delegate: ItemDelegate {
            icon.source: folderModel.isFolder(index) ? "qrc:/assets/icons/folder-fill.svg" : "qrc:/assets/icons/file-fill.svg"
            width: ListView.view.width - 20
            text: fileName
            checkable: true
            onClicked: {
                fileFolderView.currentIndex = index;
                fileFolderDialog.fileUrl = fileFolderDialog.folder + "/" + fileName
            }
        }
        highlight: Rectangle { color: "lightsteelblue" }
        ScrollBar.vertical: ScrollBar {
            width: 20
            policy: ScrollBar.AlwaysOn
        }
    }
}
