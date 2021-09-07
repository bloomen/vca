import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: vcaSettings
    color: "grey"
    radius: Style.radius
    height: column.height
    property var root_dirs

    function newDirectory()
    {

    }

    VcaOutput {
        id: daemon_hosts
        endpoint: "/daemon"
        node: "daemon_hosts"
    }

    VcaOutput {
        id: daemon_ports
        endpoint: "/daemon"
        node: "daemon_ports"
    }

    onVisibleChanged: {
        if (visible) {
            root_dirs = vcaView.getHostDirectories(daemon_hosts.value[0], daemon_ports.value[0])
        }
    }

    ColumnLayout {
        id: column
        spacing: 10

        VcaLabel {
            anchors.margins: 10
            text: "+"
            font.pointSize: Style.fontSizeHuge
            color: Style.colorPrimary
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: vcaSettings.newDirectory()
                onEntered: parent.opacity = 0.7
                onExited: parent.opacity = 1
            }
        }

        Repeater {
            model: root_dirs ? root_dirs.length : 0
            VcaLabel {
                text: root_dirs[index]
            }
        }
    }
}
