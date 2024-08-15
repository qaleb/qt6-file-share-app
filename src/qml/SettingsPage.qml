import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Rectangle {
    id: settingsPage
    color: theme.color6
    focus: true

    signal back()

    function refreshColor() {
        picker.setColor(theme.color2);
    }

    MouseArea {
        anchors.fill: parent
    }

    IconImage {
        id: backIcon
        sourceSize.width: 40
        sourceSize.height: 40
        source: "qrc:/assets/icons/ArrowBack.svg"
        color: "gray"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
        anchors.leftMargin: 5

        MouseArea {
            anchors.fill: parent
            onClicked: settingsPage.back();
        }
    }

    SmoothText {
        id: boxTitle
        anchors{
            left: backIcon.right
            top: parent.top
            leftMargin: 15
            topMargin: 5
        }

        font.pixelSize: 64
        text: qsTr("Settings")
        color: theme.color3
    }

    SText {
        id: labelPath
        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 17
            topMargin: 70
        }
        font.pixelSize: 16
        text: qsTr("Save received file in:")
        color: theme.color5
    }

    Rectangle {
        id: textPath
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelPath.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 17
        anchors.topMargin: 8
        height: 30
        color: theme.color2
        clip: true

        Image {
            anchors {
                top: parent.top
                left: parent.left
            }
            source: "qrc:/assets/icons/PanelGradient.png"
        }

        SText {
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.fill: parent
            horizontalAlignment: "AlignLeft"
            verticalAlignment: "AlignVCenter"
            elide: "ElideMiddle"
            font.pixelSize: 12
            text: guiBehind.currentPath // Display the current path
        }
    }

    ButtonDark {
        id: buttonPath
        anchors.right: parent.right
        anchors.rightMargin: 17
        anchors.top: textPath.bottom
        anchors.topMargin: 10
        label: qsTr("Change folder")
        onClicked: folderDialog.open()  // Open the FileDialog
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Change Folder")
        options: FolderDialog.ShowDirsOnly
        currentFolder: guiBehind.currentPath
        onAccepted: {
            guiBehind.changeDestinationFolder(folderDialog.selectedFolder);
        }
    }

    SText {
        id: labelBuddyName
        anchors{
            left: parent.left
            top: buttonPath.bottom
            leftMargin: 17
            topMargin: 15
        }
        font.pixelSize: 16
        text: qsTr("Appear to your buddies as:")
        color: theme.color5
    }

    Rectangle {
        id: rectBuddyName
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelBuddyName.bottom
        anchors.leftMargin: 17
        anchors.rightMargin: 17
        anchors.topMargin: 8
        height: 30
        color: theme.color2
        clip: true

        Image {
            anchors {
                top: parent.top
                left: parent.left
            }
            source: "qrc:/assets/icons/PanelGradient.png"
        }

        STextInput {
            id: textBuddyName
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 7
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.fill: parent
            horizontalAlignment: "AlignLeft"
            font.pixelSize: 12
            text: "PHAN-TEST"
            // text: guiBehind.buddyName

            // onAccepted: {
            //     // Save the buddyName when editing is finished
            //     guiBehind.setBuddyName(text);
            // }
        }

        //Binding {
        //    target: guiBehind
        //    property: "remoteDestinationAddress"
        //    value: textBuddyName.text
        //}
    }

    SText {
        id: labelColor
        anchors {
            left: labelPath.left
            top: rectBuddyName.bottom
            topMargin: 40
        }
        font.pixelSize: 16
        text: qsTr("Theme color:")
        color: theme.color5
    }

    ColorPicker {
        id: picker
        width: 185
        height: 163
        anchors.top: labelColor.bottom
        anchors.topMargin: 8
        anchors.left: labelColor.left
        onChanged: {
            guiBehind.changeThemeColor(colorValue);
        }
    }

    ColorBox {
        id: cbox1
        anchors.top: labelColor.bottom
        anchors.topMargin: 8
        anchors.left: picker.right
        anchors.leftMargin: 20
        color: "#248B00"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox2
        anchors.top: cbox1.top
        anchors.left: cbox1.right
        anchors.leftMargin: 15
        color: "#A80000"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox3
        anchors.top: cbox1.top
        anchors.left: cbox2.right
        anchors.leftMargin: 15
        color: "#3A6CBC"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox4
        anchors.top: cbox1.bottom
        anchors.topMargin: 15
        anchors.left: cbox1.left
        color: "#2e3436" // "#704214"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox5
        anchors.left: cbox4.right
        anchors.top: cbox4.top
        anchors.leftMargin: 15
        color: "#B77994"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox6
        anchors.top: cbox4.top
        anchors.left: cbox5.right
        anchors.leftMargin: 15
        color: "#5B2F42"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox7
        anchors.top: cbox4.bottom
        anchors.topMargin: 15
        anchors.left: cbox4.left
        color: "#353B56"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox8
        anchors.top: cbox7.top
        anchors.left: cbox7.right
        anchors.leftMargin: 15
        color: "#FB8504"
        onClicked: color => { picker.setColor(color) }
    }

    ColorBox {
        id: cbox9
        anchors.top: cbox7.top
        anchors.left: cbox8.right
        anchors.leftMargin: 15
        color: "#6D0D71"
        onClicked: color => { picker.setColor(color) }
    }

    SText {
        id: labelColorHexCode
        color: "#888888"
        // text: qsTr("HEX:")
        anchors {
            left: parent.left
            top: cbox7.bottom
            topMargin: 14
            leftMargin: 181
        }
        font.pixelSize: 14
    }

    Rectangle {
        id: rectColorHexCode
        anchors.top: cbox7.bottom
        anchors.topMargin: 7
        anchors.left: picker.right
        anchors.leftMargin: 20
        anchors.right: cbox9.right
        height: 30
        color: theme.color2
        clip: true

        STextInput {
            id: textColorHexCode
            anchors.fill: parent
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            leftPadding: 7
            text: theme.color2.toUpperCase()

            onTextEdited: {
                // Check if the entered text is a valid color code
                var validColor = /^#[0-9A-Fa-f]{6}$/; // A regex to validate a hex color code

                if (validColor.test(text)) {
                    // If it's a valid color code, update the userEnteredColor property
                    if(theme.color2.toUpperCase() !== text.toUpperCase()) {
                        picker.setColor(text); // Update the theme color
                    }
                }
            }
        }
    }
}
