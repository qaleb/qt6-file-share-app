import QtQuick 2.15

Item {
    clip: true

    Rectangle {
        anchors.fill: parent
        color: "white"

        Text {
            anchors.centerIn: parent
            text: qsTr("We are building a modern file sharing app.")
            color: "blue"
            font.pixelSize: 36
            scale: 0.5
        }
    }
}
