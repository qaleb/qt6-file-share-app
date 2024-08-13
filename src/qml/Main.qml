import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: rootWindow
    width: 360
    height: 600
    visible: true
    color: "#ffffff"
    maximumHeight: 600
    maximumWidth: 360
    minimumHeight: 600
    minimumWidth: 360
    title: qsTr("DropIt")

    signal showIpList()
    signal showSettings()

    FontLoader {
        id: duktofont
        source: "qrc:/assets/fonts/Klill-Light.ttf"
    }

    FontLoader {
        id: duktofontsmall
        source: "qrc:/assets/fonts/LiberationSans-Regular.ttf"
    }

    FontLoader {
        id: duktofonthappy
        source: "qrc:/assets/fonts/KGLikeASkyscraper.ttf"
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
        onShowIpList: handleBottomToolBarClick("ip")
        onShowSettings: handleBottomToolBarClick("settings")
    }

    DuktoOverlay {
        id: duktoOverlay
        anchors.fill: parent
    }

    // JavaScript functions
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

    function handleBottomToolBarClick(command) {
        if (command === "ip") {
            duktoOverlay.state = "ip";
        } else if (command === "settings") {
            duktoOverlay.state = "settings";
        }
    }

    function handleCloseRequest() {
        // Manages Android Back-button
        if (Qt.platform.os === "android") {
            if (duktoOverlay.state !== "") {
                duktoOverlay.state = ""
                close.accepted = false
            } else {
                close.accepted = true
            }
        } else {
            close.accepted = true
        }
    }

    onClosing: function(close) {
        if (Qt.platform.os === "android") {
            if (duktoOverlay.state !== "") {
                duktoOverlay.state = ""
                close.accepted = false
            } else {
                close.accepted = true
            }
        } else {
            close.accepted = true
        }
    }


    // theme.color1: #000000
    // theme.color2: #248b00
    // theme.color3: #4cb328
    // theme.color4: #555555
    // theme.color5: #888888
    // theme.color6: #ffffff
    // theme.color7: #cccccc
    // theme.color8: #eeeeee
    // theme.color9: #ccffffff
}