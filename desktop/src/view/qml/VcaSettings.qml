import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1

Rectangle {
    id: vcaSettings
    height: row.height
    radius: Style.radius
    border.color: Style.colorPrimary
    border.width: 1
    property var root_dirs
    property bool editing: false
    property var oldDir

    FolderDialog {
        id: folderDialog
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            if (vcaSettings.editing) {
                removeDirectory(vcaSettings.oldDir);
                vcaSettings.editing = false;
            }
            addDirectory(currentFolder);
        }
    }

    function updateDirectories() {
        root_dirs = vcaView.getHostDirectories(daemon_hosts.value[0], daemon_ports.value[0])
    }

    function addDirectory(dir) {
        vcaView.addHostDirectory(daemon_hosts.value[0], daemon_ports.value[0], dir);
        updateDirectories();
    }

    function removeDirectory(dir) {
        vcaView.removeHostDirectory(daemon_hosts.value[0], daemon_ports.value[0], dir);
        updateDirectories();
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
            updateDirectories();
        }
    }

    RowLayout {
        id: row
        Item {
            width: 8
        }

        ColumnLayout {
            id: column
            spacing: 10

            VcaLabel {
                text: "+"
                font.pointSize: Style.fontSizeHuge
                font.bold: true
                color: Style.colorPrimary
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: folderDialog.open()
                    onEntered: parent.opacity = 0.7
                    onExited: parent.opacity = 1
                }
            }

            Column {
                Repeater {
                    anchors.fill: parent
                    model: root_dirs ? root_dirs.length : 0
                    Row {
                        spacing: 10
                        VcaLabel {
                            text: "✘"
                            color: Style.colorPrimary
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: removeDirectory(root_dirs[index])
                                onEntered: parent.opacity = 0.7
                                onExited: parent.opacity = 1
                            }
                        }
                        VcaLabel {
                            text: "✐"
                            font.pointSize: Style.fontSizeLarge
                            rotation: 180
                            color: Style.colorPrimary
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    vcaSettings.editing = true;
                                    vcaSettings.oldDir = root_dirs[index];
                                    folderDialog.open();
                                }
                                onEntered: parent.opacity = 0.7
                                onExited: parent.opacity = 1
                            }
                        }
                        VcaTextEdit {
                            text: root_dirs[index]
                            color: Style.colorTertiary
                        }
                    }
                }
            }

            Item {
                height: 1
            }
        }
    }
}
