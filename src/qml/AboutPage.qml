import QtQuick 2.15

Item {
    clip: true

    function handleLinkActivated(link) {
        Qt.openUrlExternally(link);
    }

    Rectangle {
        anchors.fill: parent
        color: "white"

        Rectangle {
            x: 27
            y: 15
            width: 64
            height: 64
            color: theme.color2
            Image {
                source: "qrc:/assets/icons/DuktoMetroIcon.svg"
                anchors.fill: parent
                sourceSize.width: parent.width
                fillMode: Image.PreserveAspectFit
                anchors.margins: 5
            }
        }

        SmoothText {
            y: 81
            x: 25
            font.pixelSize: 100
            text: "Dukto R6"
            color: "#000"
        }
        SmoothText {
            y: 137
            x: 25
            text: qsTr("Version: 2.2024.0.2")
            font.pixelSize: 28
            color: "#000"
        }
        SmoothText {
            x: 25
            y: 70 + 100
            font.pixelSize: 38
            text: "Created by Emanuele Colombo"
            color: "#000"
        }
        SmoothText {
            x: 25
            y: 120 + 140
            font.pixelSize: 38
            text: "Updated by Caleb Maangi"
            color: "#000"
        }
        SmoothText {
            x: 25
            y: 120 + 165
            font.pixelSize: 32
            text: "QT6: <a href=\"https://github.com/qaleb/qt6-file-share-app\">https://github.com/qaleb/dukto6</a>"
            onLinkActivated: handleLinkActivated('https://github.com/qaleb/qt6-file-share-app')
            HoverHandler {
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                cursorShape: Qt.PointingHandCursor
            }
            color: "#000"
        }
        SmoothText {
            x: 25
            y: 100 + 100
            font.pixelSize: 32
            text: qsTr("Website") + ": <a href=\"http://www.msec.it/dukto/\">http://www.msec.it/dukto/</a>"
            onLinkActivated: handleLinkActivated('https://www.msec.it/dukto')
            HoverHandler {
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                cursorShape: Qt.PointingHandCursor
            }
            color: "#000"
        }
        SmoothText {
            x: 25
            y: 120 + 100
            font.pixelSize: 32
            text: "QT5: <a href=\"https://github.com/coolshou/dukto\">https://github.com/coolshou/dukto</a>"
            onLinkActivated: handleLinkActivated('https://github.com/coolshou/dukto')
            HoverHandler {
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                cursorShape: Qt.PointingHandCursor
            }
            color: "#000"
        }

        SText {
            anchors.right: parent.right
            anchors.rightMargin: 41
            anchors.left: parent.left
            anchors.leftMargin: 25
            y: 325
            font.pixelSize: 12
            color: "#000"
            wrapMode: "WordWrap"
            text: qsTr("This application and it's source code are released freely as open source project.<br>If you like this application please consider <a href=\"http://www.msec.it/\">making a donation</a>.")
            onLinkActivated: handleLinkActivated('https://www.msec.it')
        }
    }
}
