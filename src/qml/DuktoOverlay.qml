import QtQuick 2.15

Rectangle {
    id: duktoOverlay
    color: "#00000000"

    // state: guiBehind.showTermsOnStart ? "termspage" : ""
    state: ""

    function refreshSettingsColor() {
        settingsPage.refreshColor();
    }

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

    SendPage {
        id: sendPage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        onBack: parent.state = ""
        onShowTextPage: {
            showTextPage.setTextEditFocus();
            parent.state = "showtext";
        }
        visible: false
    }

    ShowTextPage {
        id: showTextPage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        onBack: parent.state = ""
        onBackOnSend: {
            sendPage.setDestinationFocus();
            parent.state = "send"
        }
        visible: false
    }

    ProgressPage {
        id: progressPage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        visible: false
    }

    MessagePage {
        id: messagePage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        onBack: parent.state = backState
        visible: false
    }

    TermsPage {
        id: termsPage
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0
        onOk: {
            guiBehind.showTermsOnStart = false;
            parent.state = ""
        }
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
        },
        State {
            name: "send"
            PropertyChanges {
                target: sendPage
                opacity: 1
                visible: true
            }
            PropertyChanges {
                target: duktoOverlay
                color: theme.color6
            }
        },
        State {
            name: "showtext"
            PropertyChanges {
                target: showTextPage
                opacity: 1
                visible: true
            }
            PropertyChanges {
                target: duktoOverlay
                color: theme.color6
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

