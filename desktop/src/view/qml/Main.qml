import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Window {
    visible: true
    minimumWidth: 640
    minimumHeight: 480
    title: vcaModel.appName()
    color: Style.colorBackground

    VcaOutput {
        id: connected
        endpoint: "/status"
        node: "connected"
    }

    VcaOutput {
        id: indexing
        endpoint: "/status"
        node: "indexing"
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

    TextEdit{
        id: textEdit
        visible: false
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Item {
                Layout.fillWidth: true
            }

            VcaLabel {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Find Anything!")
                color: Style.colorPrimary
            }

            Item {
                Layout.fillWidth: true
            }

            VcaLabel {
                id: statusLabel
                Layout.alignment: Qt.AlignRight
                text: "⬤"
                color: {
                    if (!connected.value) {
                        return Style.colorDisabled;
                    }
                    return Style.colorAlert;
                }
                SequentialAnimation {
                    running: connected.value && indexing.value
                    loops: Animation.Infinite
                    NumberAnimation { target: statusLabel; property: "opacity"; from: 0; to: 1; duration: 1000 }
                    NumberAnimation { target: statusLabel; property: "opacity"; from: 1; to: 0; duration: 1000 }
                    onStopped: {
                        statusLabel.opacity = 1;
                    }
                }
            }

            Item {
                Layout.preferredWidth: 2
            }
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

        ColumnLayout {

            VcaSettings {
                id: settings
                visible: settingsToggle.toggled
                Layout.fillWidth: true
            }

            Item {
                height: 8
            }

            ScrollView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Column {
                    Repeater {
                        model: result_count.value
                        delegate: Row {
                            id: currentRow
                            spacing: 10
                            property var path: vcaView.joinPaths(result_dirs.value[index], result_files.value[index])
                            VcaTextEdit {
                                text: result_files.value[index] ? result_files.value[index] : ""
                            }
                            VcaLabel {
                                text: "↗"
                                font.pointSize: Style.fontSizeNormal
                                color: Style.colorPrimary
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        vcaView.showFile(result_dirs.value[index], result_files.value[index]);
                                        parent.opacity = 1;
                                    }
                                    onEntered: parent.opacity = 0.7
                                    onExited: parent.opacity = 1
                                }
                            }
                            VcaLabel {
                                text: "⎘"
                                font.pointSize: Style.fontSizeNormal
                                color: Style.colorPrimary
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        textEdit.text = currentRow.path;
                                        textEdit.selectAll();
                                        textEdit.copy();
                                        parent.opacity = 1;
                                    }
                                    onEntered: parent.opacity = 0.7
                                    onExited: parent.opacity = 1
                                }
                            }
                            VcaTextEdit {
                                text: result_dirs.value[index] ? result_dirs.value[index] : ""
                                color: Style.colorTertiary
                            }
                        }
                    } // Repeater
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

}
