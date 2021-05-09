import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Column {
        spacing: 20

        Button {
            text: "button1"
            property int value
            onClicked: {
                value += 1;
                appModel.update("/settings/foo", value);
            }
            Component.onCompleted: {
                value = appModel.defaultValue("/settings/foo")
            }
        }

        Label {
            AppOutput {
                endpoint: "/settings/foo"
                onUpdated: parent.text = value
            }
        }

        Label {
            AppOutput {
                endpoint: "/settings/bar"
                onUpdated: parent.text = value
            }
        }

        Slider {
            minimumValue: 1
            maximumValue: 100
            AppInput {
                endpoint: "/settings/bar"
                value: parent.value
            }
            Component.onCompleted: {
                value = appModel.defaultValue("/settings/bar")
            }
        }

    }
}
