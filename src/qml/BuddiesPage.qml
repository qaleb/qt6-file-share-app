import QtQuick 2.15

Item {
    clip: true
    Rectangle {
        anchors.fill: parent
        color: "white"

        Text {
            anchors.centerIn: parent
            text: qsTr("Hey buddies, this is your page.")
            color: "red"
            font.pixelSize: 36
            scale: 0.5
        }
    }
}
