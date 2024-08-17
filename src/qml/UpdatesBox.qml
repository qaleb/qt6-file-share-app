import QtQuick 2.15

Item {
    id: box
    width: 190
    height: 55
    clip: true
    state: guiBehind.showUpdateBanner ? "showed" : "hidden"

    Rectangle {
        id: backRecangle
        color: theme.color2
        height: parent.height
        width: parent.width

        Image {
            anchors.fill: parent
            source: "qrc:/assets/icons/TileGradient.png"
        }

        SText {
            id: labelText
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: 14
            wrapMode: "WordWrap"
            text: qsTr("A new release is available, click here to download it!")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: Qt.openUrlExternally("http://www.msec.it/dukto/r5update.php")
        }

        Behavior on x { NumberAnimation { duration: 500; easing.type: "OutCubic" } }
    }
    states: [
        State {
            name: "hidden"

            PropertyChanges {
                target: backRecangle
                x: box.width
            }
        },
        State {
            name: "showed"

            PropertyChanges {
                target: backRecangle
                x: 0
            }
        }
    ]
}
