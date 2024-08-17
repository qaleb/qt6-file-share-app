import QtQuick 2.15

Rectangle {
    id: progressPage
    color: "#00000000"

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

        SmoothText {
            id: boxTitle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 15
            anchors.topMargin: 5
            font.pixelSize: 64
            text: guiBehind.currentTransferSending ? qsTr("Sending data") : qsTr("Receiving data")
        }

        SText {
            id: boxSender
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 17
            anchors.right: progressBar.right
            anchors.topMargin: 45
            elide: "ElideRight"
            font.pixelSize: 16
            text: (guiBehind.currentTransferSending ? qsTr("to ") : qsTr("from ")) + guiBehind.currentTransferBuddy
        }

        Rectangle {
            id: progressBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: boxSender.bottom
            anchors.leftMargin: 17
            anchors.rightMargin: 17
            anchors.topMargin: 25
            color: theme.color3
            height: 40

            Rectangle {
                id: progressFiller
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                color: theme.color6
                width: parent.width * guiBehind.currentTransferProgress / 100;
            }
        }

        Button {
            id: abortButton
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 17
            anchors.bottomMargin: 17
            width: 75
            label: qsTr("Abort")
            onClicked: guiBehind.abortTransfer()
            visible: guiBehind.currentTransferSending
        }

        SText {
            id: statusText
            anchors.left: parent.left
            anchors.right: abortButton.left
            anchors.verticalCenter: abortButton.verticalCenter
            anchors.leftMargin: 17
            anchors.rightMargin: 17
            font.pixelSize: 17
            elide: "ElideRight"
            text: guiBehind.currentTransferStats
        }
    }
}
