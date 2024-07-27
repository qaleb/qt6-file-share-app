import QtQuick
import QtQuick.Controls

Window {
    width: 360
    height: 600
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: mainRect
        width: 100; height: 100
        anchors.centerIn: parent
        color: '#FFBB33'
    }

    Rectangle {
        id: smallRect
        width: 50; height: 50
        anchors.centerIn: parent
        color: "#0c30e4"
    }

    Rectangle { // our inlined button ui
        id: button
        anchors.top: mainRect.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        width: 116; height: 26
        color: "lightsteelblue"
        border.color: "slategrey"
        Text {
            anchors.centerIn: parent
            text: "Start"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                status.text = "Button clicked!"
            }
        }
    }

    Text {
        id: status
        anchors.top: button.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: 116; height: 26
        text: "waiting ..."
        horizontalAlignment: Text.AlignHCenter
    }



    Rectangle {
        id: whiteRectBackground
        width: 50; height: 50
        anchors.centerIn: parent
        color: "#dee3fa"
        anchors.verticalCenterOffset: 183
        anchors.horizontalCenterOffset: 0

        Image {
            id: imageWhite
            anchors.centerIn: parent
            width: 40; height: 40
            source: "qrc:/assets/icons/SettingsDark.svg"
        }
    }

    Rectangle {
        id: darkRectBackground
        width: 50; height: 50
        anchors.centerIn: parent
        color: "#0c30e4"
        anchors.verticalCenterOffset: 257
        anchors.horizontalCenterOffset: 0

        Image {
            id: imageDark
            anchors.centerIn: parent
            width: 40; height: 40
            source: "qrc:/assets/icons/SettingsWhite.svg"
        }
    }
}
