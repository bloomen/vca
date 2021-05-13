import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Rectangle {
    id: button
    radius: Style.radius
    height: 25
    width: Math.max(50, label.contentWidth + 20)

    enabled: true
    property bool hovered: false
    property bool pressed: false

    property string text: ""

    property color baseColor: Style.colorPrimary

    signal clicked

    color: {
        if (enabled) {
            if (pressed) {
                return "white";
            }
            else if (hovered) {
                return button.baseColor;
            }
            else {
                return Style.dim(button.baseColor);
            }
        }
        else {
            return Style.disable(button.baseColor);
        }
    }

    border.width: {
        if (button.enabled && button.pressed) {
            return 1;
        }
        else {
            return 0;
        }
    }

    border.color: button.baseColor

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            button.hovered = true;
        }
        onExited: {
            button.hovered = false;
            button.pressed = false;
        }
        onPressed: {
            button.pressed = true;
        }
        onReleased: {
            if (button.pressed) {
                button.pressed = false;
                button.clicked();
            }
        }
    }

    VcaLabel {
        id: label
        anchors.fill: parent
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        text: button.text
        color: {
            if (button.enabled && button.pressed) {
                return button.baseColor;
            }
            else {
                return "white";
            }
        }
    }
}
