import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: messagePage
    color: "#00000000"

    signal back(string backState)

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        height: 200

        Rectangle {
            id: backRecangle
            color: theme.color2
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 600
        }

        Image {
            source: "qrc:/assets/icons/BottomShadow.png"
            anchors.bottom: backRecangle.top
            anchors.left: backRecangle.left
            anchors.right: backRecangle.right
            fillMode: Image.TileHorizontally
        }

        Image {
            source: "qrc:/assets/icons/TopShadow.png"
            anchors.top: backRecangle.bottom
            anchors.left: backRecangle.left
            anchors.right: backRecangle.right
            fillMode: Image.TileHorizontally
        }

        Image {
            anchors.top: parent.top
            anchors.left: parent.left
            source: "qrc:/assets/icons/PanelGradient.png"
        }

        IconImage {
            id: backIcon
            sourceSize.width: 40
            sourceSize.height: 40
            source: "qrc:/assets/icons/ArrowBack.svg"
            color: theme.color6
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 5
            anchors.leftMargin: 5

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    guiBehind.resetProgressStatus();
                    messagePage.back(guiBehind.messagePageBackState);
                }
            }
        }

        SmoothText {
            id: boxTitle
            anchors {
                left: backIcon.right
                top: parent.top
                leftMargin: 15
                topMargin: 5
            }
            font.pixelSize: 64
            text: guiBehind.messagePageTitle
        }

        SText {
            anchors {
                top: backIcon.bottom
                left: boxTitle.left
                right: parent.right
                bottom: parent.bottom
                topMargin: 20
                bottomMargin: 10
                rightMargin: 5
            }
            font.pixelSize: 14
            wrapMode: Text.Wrap
            text: guiBehind.messagePageText
        }
    }
}
