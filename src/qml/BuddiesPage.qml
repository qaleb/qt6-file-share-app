import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    clip: true

    Rectangle {
        anchors.fill: parent
        color: theme.color6
    }

    DropArea {
        id: dropTarget
        anchors.fill: parent
        visible: guiBehind.overlayState === "" && guiBehind.canAcceptDrop()

        onDropped: function(drop) {
            if(guiBehind.canAcceptDrop()){
                if (drop.hasUrls) {
                    var filesList = drop.urls.map(function(url) { return url.toString(); });
                    // Pass the list of file URLs to the sendDroppedFiles function
                    guiBehind.sendDroppedFiles(filesList);
                }
            }
        }
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

        onCountChanged: {
            if(buddiesList.count === 3){
                dropTarget.visible = true
            } else {
                dropTarget.visible = false
            }
        }
    }
}
