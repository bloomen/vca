import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: vcaModel.appName() + qsTr("- find anything!")

    Column {
        anchors.centerIn: parent
        spacing: 20

        VcaInputField {
            width: 400
            placeholderText: qsTr("What are you looking for?")
            onTextChanged: {
                vcaModel.setValue("/search", "input", text);
            }
        }

        VcaLabel {
            VcaOutput {
                endpoint: "/search"
                node: "result"
                onUpdated: parent.text = value
            }
        }

    }
}
