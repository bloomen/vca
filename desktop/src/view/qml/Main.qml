import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

Window {
    visible: true
    minimumWidth: 640
    minimumHeight: 480
    title: vcaModel.appName() + qsTr(" - find anything!")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 30

        RowLayout {
            Layout.fillWidth: true
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
                Layout.preferredWidth: 20
                text: "⚙"
                font.pointSize: Style.fontSizeHuge
                color: Style.colorPrimary
            }

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
