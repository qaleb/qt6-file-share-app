import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: showTextPage
    color: theme.color6

    signal back()
    signal backOnSend()

    property alias textInputFocus: textEditSnippet.focus

    MouseArea {
        anchors.fill: parent
    }

    IconImage {
        id: backIcon
        sourceSize.width: 40
        sourceSize.height: 40
        source: "qrc:/assets/icons/ArrowBack.svg"
        color: theme.color5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
        anchors.leftMargin: 5

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (guiBehind.textSnippetSending)
                    showTextPage.backOnSend();
                else
                    showTextPage.back();
            }
        }
    }

    SmoothText {
        id: boxTitle
        anchors {
            left: backIcon.right
            top: parent.top
            leftMargin: 15
            topMargin: 5
        }
        font.pixelSize: 64
        text: qsTr("Text snippet")
        color: theme.color3
    }

    SText {
        id: boxSender
        anchors {
            left: backIcon.right
            top: parent.top
            leftMargin: 17
            right: parent.right
            rightMargin: 20
            topMargin: 45
        }
        elide: "ElideRight"
        font.pixelSize: 16
        text: (guiBehind.textSnippetSending ? qsTr("to ") : qsTr("from ")) + guiBehind.textSnippetBuddy
        color: theme.color5
    }


    Rectangle {
        id: rectangleText
        border.color: theme.color3
        border.width: 1
        radius: 5
        anchors {
            top: boxSender.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 15
            bottom: buttonCopy.top
            bottomMargin: 20
            right: parent.right
            rightMargin: 10
        }

        Flickable {
            id: flickableText
            anchors.fill: parent
            anchors.margins: 5
            contentHeight: textEditSnippet.contentHeight
            flickableDirection: Flickable.VerticalFlick

            clip: true

            function ensureVisible(r) {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: textEditSnippet
                color: theme.color4
                anchors.fill: parent
                anchors.margins: 5
                font.family: duktofontsmall.name
                font.pixelSize: 13
                selectByMouse: false // Disabled due to touch devices, highlights text while scrolling
                wrapMode: TextEdit.Wrap
                textFormat: TextEdit.PlainText
                readOnly: guiBehind.textSnippetSending ? false : true
                text: guiBehind.textSnippet
                onCursorRectangleChanged: flickableText.ensureVisible(cursorRectangle)
            }
        }
    }

    ButtonDark {
        id: buttonCopy
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        buttonEnabled: guiBehind.textSnippetSending ? guiBehind.clipboardTextAvailable : true
        label: guiBehind.textSnippetSending ? qsTr("Paste from clipboard") : qsTr("Copy to clipboard")
        onClicked: {
            if (guiBehind.textSnippetSending)
                textEditSnippet.paste();
            else {
                if (textEditSnippet.selectedText === "")
                    textEditSnippet.selectAll();
                textEditSnippet.copy();
            }
        }
    }

    ButtonDark {
        id: buttonSend
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 15
        anchors.bottomMargin: 10
        label: qsTr("Send")
        visible: guiBehind.textSnippetSending
        buttonEnabled: textEditSnippet.text !== ""
        onClicked: {
            guiBehind.textSnippet = textEditSnippet.text;
            guiBehind.sendText();
        }
    }
}
