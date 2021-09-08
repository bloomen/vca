import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Window {
    visible: true
    minimumWidth: 640
    minimumHeight: 480
    title: vcaModel.appName()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20

        VcaLabel {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Find Anything!")
            color: Style.colorPrimary
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop

            VcaInputField {
                Layout.fillWidth: true
                border.color: Style.colorPrimary
                border.width: 1
                Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                placeholderText: qsTr("What are you looking for?")
                onTextChanged: {
                    vcaModel.setValue("/search", "input", text);
                }
            }

            VcaLabel {
                id: settingsToggle
                property bool toggled: false
                Layout.preferredWidth: 20
                text: "⚙"
                font.pointSize: Style.fontSizeHuge
                color: toggled ? Style.colorTertiary : Style.colorPrimary
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: parent.toggled = !parent.toggled
                    onEntered: parent.opacity = 0.7
                    onExited: parent.opacity = 1
                }
            }
        }

        VcaSettings {
            visible: settingsToggle.toggled
            Layout.fillWidth: true
        }

        VcaOutput {
            id: result_count
            endpoint: "/search"
            node: "result_count"
        }

        VcaOutput {
            id: result_files
            endpoint: "/search"
            node: "result_files"
        }

        Item {
            // filler item
            Layout.fillHeight: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: result_count.value
                VcaLabel {
                    text: result_files.value[index] ? result_files.value[index] : ""
                }
            }
        }
    }

}
