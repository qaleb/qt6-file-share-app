import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    state: "buddies"
    color: theme.color6

    signal showIpList()
    signal showSettings()

    // JavaScript functions
    function gotoPage(page) {
        tabBar.state = page;
        state = page;
    }

    function handleTabClick(tab) {
        if (tab === "buddies") {
            swipeView.currentIndex = 0;
        } else if (tab === "recent") {
            swipeView.currentIndex = 1;
        } else if (tab === "about") {
            swipeView.currentIndex = 2;
        }
    }

    function handleSwipeViewIndexChanged(currentIndex) {
        if (currentIndex === 0) {
            topTabBar.state = "buddies";
        } else if (currentIndex === 1) {
            topTabBar.state = "recent";
        } else if (currentIndex === 2) {
            topTabBar.state = "about";
        }
    }

    TopTabBar {
        id: topTabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        onClicked: (tab) => handleTabClick(tab)
    }

    SwipeView {
        id: swipeView
        anchors.top: topTabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Page {
            Loader {
                id: buddiesPage
                anchors.fill: parent
                source: "BuddiesPage.qml"
            }
        }

        Page {
            Loader {
                anchors.fill: parent
                source: "RecentPage.qml"
            }
        }

        Page {
            Loader {
                anchors.fill: parent
                source: "AboutPage.qml"
            }
        }

        Component.onCompleted: {
            contentItem.highlightMoveDuration = 600 // Set the moving time to 600 ms
            contentItem.highlightMoveVelocity = 10
        }

        onCurrentIndexChanged: handleSwipeViewIndexChanged(swipeView.currentIndex)
    }

    BottomToolBar {
        id: bottomToolBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        onShowIpList: parent.showIpList()
        onShowSettings: parent.showSettings()
    }

    transitions: [
        Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuart; duration: 1000 }
            NumberAnimation { properties: "opacity"; easing.type: Easing.InQuad; duration: 500 }
        }
    ]
}

