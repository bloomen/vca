import QtQuick 2.9

Item {
    id: image
    property string source
    property int sourceHeight: 200
    property int sourceWidth: 200
    property real hoveredOffOpacity: 1
    property string link: ""

    Image {
        id: img
        anchors.fill: parent
        sourceSize.height: image.sourceHeight
        sourceSize.width: image.sourceWidth
        source: "/view/img/" + image.source
        opacity: image.hoveredOffOpacity
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            img.opacity = 1
        }
        onExited: {
            img.opacity = image.hoveredOffOpacity
        }
        onClicked: {
            if (image.link.length > 0) {
                Qt.openUrlExternally(image.link);
            }
        }
    }
}
