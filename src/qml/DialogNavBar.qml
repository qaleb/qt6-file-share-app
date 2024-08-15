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
            id: showIpIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -( parent.width / 6)
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/ArrowBack.svg"
            color: theme.color6
            MouseArea {
                anchors.fill: parent
                onClicked: bottomNavBar.back()
            }
        }

        Text {
            anchors {
                top: showIpIcon.bottom
                topMargin: 3
                horizontalCenter: showIpIcon.horizontalCenter
            }

            text: qsTr("Back")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            width: 1
            color: theme.color6
        }

        IconImage {
            id: settingsIcon
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: ( parent.width / 6)
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/ArrowUp.svg"
            color: theme.color6

            // MouseArea {
            //     anchors.fill: parent
            //     onClicked: showSettings();
            // }
        }

        Text {
            anchors{
                top: settingsIcon.bottom
                topMargin: 3
                horizontalCenter: settingsIcon.horizontalCenter
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
                left: settingsIcon.right
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
