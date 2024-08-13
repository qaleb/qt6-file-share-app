import QtQuick 2.15

Item {
    id: topTabBar
    height: 65
    state: "buddies"

    signal clicked(string tab)
    property string themeColor: theme.color2

    Item {
        anchors.fill: parent

        SmoothText {
            id: buddyText
            x: 10
            font.pixelSize: 84
            text: qsTr("buddies")
            width: buddyText.text.width
            transformOrigin: Item.TopLeft
            color: buddyTextMouseArea.containsMouse ? themeColor : "#9c9c9c"
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

        SmoothText {
            id: recentText
            x: 180
            font.pixelSize: 84
            text: qsTr("recent")
            width: recentText.text.width
            transformOrigin: Item.TopLeft
            color: recentTextMouseArea.containsMouse ? themeColor : "#9c9c9c"
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

        SmoothText {
            id: aboutText
            x: 310
            font.pixelSize: 84
            text: qsTr("about")
            width: aboutText.text.width
            transformOrigin: Item.TopLeft
            color: aboutTextMouseArea.containsMouse ? themeColor : "#9c9c9c"
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
                color: themeColor
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
                color: themeColor
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
                color: themeColor
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
