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

        VcaOutput {
            id: result_dirs
            endpoint: "/search"
            node: "result_dirs"
        }

        Item {
            // filler item
            Layout.fillHeight: true
        }

        TextEdit{
            id: textEdit
            visible: false
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                Layout.alignment: Qt.AlignTop
                model: result_count.value
                Row {
                    spacing: 10
                    VcaLabel {
                        text: result_files.value[index] ? result_files.value[index] : ""
                        color: Style.colorPrimary
                    }
                    VcaLabel {
                        text: result_dirs.value[index] ? result_dirs.value[index] : ""
                    }
                    VcaLabel {
                        text: "⎘"
                        font.pointSize: Style.fontSizeNormal
                        color: Style.colorPrimary
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                textEdit.text = vcaView.joinPaths(result_dirs.value[index], result_files.value[index]);
                                textEdit.selectAll();
                                textEdit.copy();
                                parent.opacity = 1;
                            }
                            onEntered: parent.opacity = 0.7
                            onExited: parent.opacity = 1
                        }
                    }
                }
            }
        }
    }

}
