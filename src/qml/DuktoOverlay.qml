import QtQuick 2.15

Rectangle {
    color: "#00000000"
    state: ""

    Rectangle {
        id: disabler
        anchors.fill: parent
        color: "#ccffffff"
        opacity: 0
        visible: false

        MouseArea {
            anchors.fill: parent
        }
    }

    IpPage {
        id: ipPage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        onBack: parent.state = ""
        visible: false
    }

    SettingsPage {
        id: settingsPage
        anchors.centerIn: parent
        anchors.fill: parent
        opacity: 0
        onBack: parent.state = ""
        visible: false
    }

    states: [
        State {
            name: "ip"
            PropertyChanges {
                target: ipPage
                opacity: 1
                visible: true
            }
            PropertyChanges {
                target: disabler
                opacity: 1
                visible: true
            }
        },
        State {
            name: "settings"
            PropertyChanges {
                target: settingsPage
                opacity: 1
                visible: true
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutCubic; duration: 500 }
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutCubic; duration: 500 }
        }
    ]
}

