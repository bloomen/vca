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

        VcaLabel {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHLeft | Qt.AlignTop
            VcaOutput {
                endpoint: "/search"
                node: "result"
                onUpdated: parent.text = value
            }
        }
    }
}
