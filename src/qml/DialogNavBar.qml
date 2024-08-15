import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Item {
    id: bottomNavBar
    height: 71
    anchors.bottomMargin: -18
    state: "WithoutLabels"

    Behavior on anchors.bottomMargin { NumberAnimation { duration: 200; easing.type: "OutCubic" } }

    signal back()
    signal acceptFolder()
    signal goUpFolder()

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
            id: cancelIcon
            anchors {
                top: parent.top
                topMargin: 5
                right: acceptIcon.left
                rightMargin: 50
            }
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/Cancel.svg"
            color: theme.color6
            MouseArea {
                anchors.fill: parent
                onClicked: back()
            }
        }

        Text {
            anchors {
                top: cancelIcon.bottom
                topMargin: 3
                horizontalCenter: cancelIcon.horizontalCenter
            }

            text: qsTr("Cancel")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: theme.color6
        }

        IconImage {
            id: acceptIcon
            anchors {
                top: parent.top
                topMargin: 5
                horizontalCenter: parent.horizontalCenter
            }
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/Accept.svg"
            color: theme.color6
            MouseArea {
                anchors.fill: parent
                onClicked: acceptFolder();
            }
        }

        Text {
            anchors {
                top: acceptIcon.bottom
                topMargin: 3
                horizontalCenter: acceptIcon.horizontalCenter
            }

            text: qsTr("Accept")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: theme.color6
        }

        IconImage {
            id: upIcon
            anchors {
                top: parent.top
                topMargin: 5
                left: acceptIcon.right
                leftMargin: 50
            }

            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/FolderUp.svg"
            color: theme.color6

            MouseArea {
                anchors.fill: parent
                onClicked: goUpFolder();
            }
        }

        Text {
            anchors{
                top: upIcon.bottom
                topMargin: 3
                horizontalCenter: upIcon.horizontalCenter
            }

            text: qsTr("Up")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: theme.color6
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
            color: theme.color6
        }

        MouseArea {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: upIcon.right
                leftMargin: 25
                right: parent.right
            }

            onClicked: {
                if (bottomNavBar.state === "WithoutLabels") {
                    bottomNavBar.state = "WithLabels";
                    moreIcon.opacity = 0.0;
                    moreIcon.source = "qrc:/assets/icons/SwipeDown.png";
                    moreIcon.opacity = 1.0;
                } else {
                    bottomNavBar.state = "WithoutLabels";
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
                target: bottomNavBar
                anchors.bottomMargin: -18
            }
        },
        State {
            name: "WithLabels"

            PropertyChanges {
                target: bottomNavBar
                anchors.bottomMargin: 0
            }
        }
    ]
}
