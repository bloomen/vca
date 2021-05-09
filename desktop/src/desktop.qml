import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Search Me")

    Column {
        anchors.centerIn: parent
        spacing: 20

        TextField {
            width: 400
            placeholderText: qsTr("What are you looking for?")
            onAccepted: {
                appModel.update("/search/input", text);
            }
        }

        Label {
            AppOutput {
                endpoint: "/search/output"
                onUpdated: parent.text = value
            }
        }

    }
}
