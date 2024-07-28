import QtQuick 2.15

Item {
    clip: true

    Rectangle {
        anchors.fill: parent
        color: "white"

        Text {
            anchors.centerIn: parent
            text: qsTr("You recent files are found here")
            color: "green"
            font.pixelSize: 36
            scale: 0.5
        }
    }
}
