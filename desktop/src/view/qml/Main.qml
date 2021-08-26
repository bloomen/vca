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

        VcaInputField {
            border.color: Style.colorPrimary
            border.width: 1
            width: 500
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            placeholderText: qsTr("What are you looking for?")
            onTextChanged: {
                vcaModel.setValue("/search", "input", text);
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
            Repeater {
                model: result_count.value
                Label {
                    text: result_files.value[index] ? result_files.value[index] : ""
                }
            }
        }
    }

}
