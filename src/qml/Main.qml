import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
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

    TopTabBar {
        id: topTabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        onClicked: (tab) => {
                       if (tab === "buddies") {
                           swipeView.currentIndex = 0;
                       } else if (tab === "recent") {
                           swipeView.currentIndex = 1;
                       } else if (tab === "about") {
                           swipeView.currentIndex = 2;
                       }
                   }
    }

    SwipeView {
        id: swipeView
        anchors.top: topTabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomToolBar.top

        Page {
            Loader {
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

        onCurrentIndexChanged: {
            if(currentIndex === 0){
                topTabBar.state = "buddies"
            } else if(currentIndex === 1){
                topTabBar.state = "recent"
            } else if(currentIndex === 2){
                topTabBar.state = "about"
            }
        }

        transitions: Transition {
            NumberAnimation {
                properties: AnchorChanges
                duration: 1000
                easing.type: Easing.InOutQuad
            }
        }
    }

    BottomToolBar {
        id: bottomToolBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
