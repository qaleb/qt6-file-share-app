import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    clip: true

    Rectangle {
        anchors.fill: parent
        color: "#fff"
    }

    ListModel {
        id: buddiesListData
        ListElement {
            ip: ""
            avatar: ""
            generic: "qrc:/assets/icons/WindowsLogo.png"
            username: "John Doe"
            system: "Windows 10"
            oslogo: "qrc:/assets/icons/WindowsLogo.png"
            showback: false
        }
        ListElement {
            ip: "::ffff:192.168.100.2"
            avatar: ""
            generic: "qrc:/assets/icons/AppleLogo.png"
            username: "Jane Smith"
            system: "macOS"
            oslogo: "qrc:/assets/icons/AppleLogo.png"
            showback: false
        }
        // Add more ListElements as needed
    }

    ListView {
        id: buddiesList
        anchors.fill: parent
        spacing: 10
        anchors.leftMargin: 25
        anchors.rightMargin: 0
        model: buddiesListData // Use the ListModel defined above

        Component {
            id: contactDelegate
            BuddyListComponent {
                buddyIp: ip
                buddyAvatar: avatar
                buddyGeneric: generic
                buddyUsername: username
                buddySystem: system
                buddyIpAddress: ip !== "IP" ? ip.substring(7) : ""
                buddyOsLogo: oslogo
                buddyShowBack: showback
            }
        }

        delegate: contactDelegate
    }
}
