import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Rectangle {
    id: checkBox
    radius: Style.radius
    height: 20
    width: height

    property bool checked: false

    signal clicked

    enabled: true
    property bool hovered: false
    property bool pressed: false

    color: "white"

    border.width: 1
    border.color: {
        if (checkBox.hovered) {
            return Style.textColorNormal
        }
        else {
            return Style.colorBackground
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            checkBox.hovered = true;
        }
        onExited: {
            checkBox.hovered = false;
            checkBox.pressed = false;
        }
        onPressed: {
            checkBox.pressed = true;
        }
        onReleased: {
            if (checkBox.pressed) {
                checkBox.pressed = false;
                checkBox.checked = !checkBox.checked
                checkBox.clicked();
            }
        }
    }

    VcaLabel {
        id: label
        anchors.fill: parent
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        text: checkBox.checked ? "✓"  : ""
        color: Style.textColorNormal
        font.pointSize: 14
    }
}
