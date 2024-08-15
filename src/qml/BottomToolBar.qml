import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Item {
    id: bottomToolBar
    height: 71
    anchors.bottomMargin: -18
    state: "WithoutLabels"

    signal showIpList();
    signal showSettings();

    Behavior on anchors.bottomMargin { NumberAnimation { duration: 200; easing.type: "OutCubic" } }

    Image {
        source: "qrc:/assets/icons/BottomShadow.png"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        fillMode: Image.TileHorizontally
    }

    Rectangle {
        y: 3
        id: mainRect
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height
        color: theme.color2

        IconImage {
            id: openFolderIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: showIpIcon.left
            anchors.rightMargin: 50
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/OpenFolder.svg"
            color: "#fff"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    folderDialog.open();
                }
            }
        }

        Text {
            anchors.top: openFolderIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: openFolderIcon.horizontalCenter
            text: qsTr("Received")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: "#fff"
        }

        FolderDialog {
            id: folderDialog
            title: qsTr("Change Folder")
            options: {
                FolderDialog.ShowDirsOnly;
                FolderDialog.ReadOnly;
            }
            currentFolder: guiBehind.currentPath
        }

        IconImage {
            id: showIpIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/ShowIpDark.svg"
            color: "#fff"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    guiBehind.refreshIpList();
                    showIpList();
                }
            }
        }

        Text {
            anchors.top: showIpIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: showIpIcon.horizontalCenter
            text: qsTr("Addresses")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: "#fff"
        }

        IconImage {
            id: settingsIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: showIpIcon.right
            anchors.leftMargin: 50
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/SettingsDark.svg"
            color: "#fff"

            MouseArea {
                anchors.fill: parent
                onClicked: showSettings();
            }
        }

        Text {
            anchors.top: settingsIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: settingsIcon.horizontalCenter
            text: qsTr("Settings")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: "#fff"
        }

        IconImage {
            id: moreIcon
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 20
            width: 20; height: 20
            source: "qrc:/assets/icons/SwipeUp.png"
            opacity: 1.0
            color: "#fff"
        }

        MouseArea {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: settingsIcon.right
            anchors.leftMargin: 25
            anchors.right: parent.right

            onClicked: {
                if (bottomToolBar.state === "WithoutLabels") {
                    bottomToolBar.state = "WithLabels";
                    moreIcon.opacity = 0.0;
                    moreIcon.source = "qrc:/assets/icons/SwipeDown.png";
                    moreIcon.opacity = 1.0;
                } else {
                    bottomToolBar.state = "WithoutLabels";
                    moreIcon.opacity = 0.0;
                    moreIcon.source = "qrc:/assets/icons/SwipeUp.png";
                    moreIcon.opacity = 1.0;
                }
            }
        }
    }

    states: [
        State {
            name: "WithoutLabels"

            PropertyChanges {
                target: bottomToolBar
                anchors.bottomMargin: -18
            }
        },
        State {
            name: "WithLabels"

            PropertyChanges {
                target: bottomToolBar
                anchors.bottomMargin: 0
            }
        }
    ]
}
