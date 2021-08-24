import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Rectangle {
    id: inputField
    color: "white"
    height: 35
    width: 200
    radius: Style.radius

    property string placeholderText: ""
    property int echoMode: TextInput.Normal
    property string text: textInput.text

    function setText(value) {
        textInput.text = value;
    }

    function clear() {
        textInput.clear();
    }

    VcaLabel {
        verticalAlignment: Qt.AlignVCenter
        anchors.fill: parent
        anchors.margins: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 30
        visible: inputField.placeholderText.length > 0 && textInput.text.length === 0
        text: inputField.placeholderText
        color: "lightgrey"
    }

    TextInput {
        id: textInput
        verticalAlignment: Qt.AlignVCenter
        anchors.fill: parent
        anchors.margins: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 30
        echoMode: inputField.echoMode
        selectByMouse: true
        selectionColor: Style.dim(Style.colorPrimary)
        clip: true
        color: Style.textColorNormal
        font.family: Style.fontRegular.name
        font.pointSize: Style.fontSizeNormal
    }

    Label {
        visible: textInput.text.length > 0
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        text: "X"
        color: Style.colorPrimary
        MouseArea {
            anchors.fill: parent
            onClicked: {
                textInput.clear();
            }
        }
    }
}
