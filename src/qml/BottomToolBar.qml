import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: bottomToolBar
    height: 71
    anchors.bottomMargin: -18

    state: "WithoutLabels"
    Behavior on anchors.bottomMargin {
        NumberAnimation {
            duration: 200
            easing.type: Easing.OutCubic
        }
    }

    Rectangle {
        id: mainRect
        anchors.fill: parent
        color: "#dee3fa"

        Image {
            id: openFolderIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: showIpIcon.left
            anchors.rightMargin: 40
            // width: 40
            // height: 40
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/OpenFolder.svg"
        }

        Text {
            anchors.top: openFolderIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: openFolderIcon.horizontalCenter
            text: qsTr("Received")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
        }

        Image {
            id: showIpIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            // width: 40
            // height: 40
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/ShowIpDark.svg"
        }

        Text {
            anchors.top: showIpIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: showIpIcon.horizontalCenter
            text: qsTr("Addresses")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
        }

        Image {
            id: settingsIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: showIpIcon.right
            anchors.leftMargin: 40
            // width: 40
            // height: 40
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/SettingsDarkNew.svg"
        }

        Text {
            anchors.top: settingsIcon.bottom
            anchors.topMargin: 3
            anchors.horizontalCenter: settingsIcon.horizontalCenter
            text: qsTr("Settings")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
        }

        Image {
            id: moreIcon
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 15
            width: 20
            height: 20
            source: "qrc:/assets/icons/SwipeUp.png"
            opacity: 1.0

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }
        }


        MouseArea {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: moreIcon.left
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
                anchors.bottomMargin: -22
            }
            PropertyChanges {
                target: moreIcon
                source: "qrc:/assets/icons/SwipeUp.png"
            }
        },
        State {
            name: "WithLabels"

            PropertyChanges {
                target: bottomToolBar
                anchors.bottomMargin: 0
            }
            PropertyChanges {
                target: moreIcon
                source: "qrc:/assets/icons/SwipeDown.png"
            }
        }
    ]

}
