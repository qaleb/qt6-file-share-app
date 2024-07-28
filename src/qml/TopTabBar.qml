import QtQuick 2.15

Item {
    id: topTabBar
    height: 65
    state: "buddies"

    signal clicked(string tab)

    Item {
        anchors.fill: parent

        Text {
            id: buddyText
            x: 10
            font.pixelSize: 84
            text: qsTr("buddies")
            width: buddyText.text.width
            transformOrigin: Item.TopLeft
            color: buddyTextMouseArea.containsMouse ? "#248b00" : "#9c9c9c"
            scale: 0.5

            MouseArea {
                id: buddyTextMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    topTabBar.state = "buddies";
                    topTabBar.clicked("buddies");
                }
            }
        }

        Text {
            id: recentText
            x: 180
            font.pixelSize: 84
            text: qsTr("recent")
            width: recentText.text.width
            transformOrigin: Item.TopLeft
            color: recentTextMouseArea.containsMouse ? "#248b00" : "#9c9c9c"
            scale: 0.5

            MouseArea {
                id: recentTextMouseArea
                anchors.fill: parent;
                hoverEnabled: true;
                onClicked: {
                    topTabBar.state = "recent";
                    topTabBar.clicked("recent");
                }
            }
        }

        Text {
            id: aboutText
            x: 310
            font.pixelSize: 84
            text: qsTr("about")
            width: aboutText.text.width
            transformOrigin: Item.TopLeft
            color: aboutTextMouseArea.containsMouse ? "#248b00" : "#9c9c9c"
            scale: 0.5

            MouseArea {
                id: aboutTextMouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    topTabBar.state = "about";
                    topTabBar.clicked("about");
                }
            }
        }
    }
    states: [
        State {
            name: "buddies"

            PropertyChanges {
                target: buddyText
                color: "#248b00"
            }
        },
        State {
            name: "recent"

            PropertyChanges {
                target: buddyText
                x: -55
            }

            PropertyChanges {
                target: recentText
                x: 100
                color: "#248b00"
            }

            PropertyChanges {
                target: aboutText
                x: 235
            }
        },
        State {
            name: "about"

            PropertyChanges {
                target: buddyText
                x: -120
            }

            PropertyChanges {
                target: recentText
                x: 52
            }

            PropertyChanges {
                target: aboutText
                x: 190
                color: "#248b00"
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuart; duration: 1000 }
            ColorAnimation  { duration: 300 }
        }
    ]
}
