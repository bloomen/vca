import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "."

Item {
    id: blocker
    visible: false

    property string text: ""
    property bool isError: false
    property bool dismissable: true

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: false
        hoverEnabled: true
    }

    Rectangle {
        anchors.fill: parent
        opacity: Style.opacityDisabled
        color: "white"
    }

    Rectangle {
        id: dialog
        visible: blocker.text.length > 0
        anchors.centerIn: parent
        height: 100
        width: height * 3
        radius: Style.radius
        color: Style.colorSecondary

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 10

            VcaLabel {
                Layout.alignment: Qt.AlignHCenter
                text: blocker.text
                color: blocker.isError ? Style.textColorError : Style.textColorNormal
            }

            Item {
                visible: blocker.dismissable
                Layout.fillHeight: true
            }

            VcaButton {
                visible: blocker.dismissable
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("OK")
                onClicked: {
                    blocker.visible = false;
                }
            }

        }
    }

    DropShadow {
        visible: dialog.visible
        anchors.fill: dialog
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: dialog
    }

}
