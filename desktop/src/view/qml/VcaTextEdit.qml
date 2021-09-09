import QtQuick 2.6
import QtQuick.Controls 1.0

TextEdit {
    cursorVisible: false
    color: Style.colorPrimary
    selectByMouse: true
    selectionColor: Style.colorPrimary
    font.family: Style.fontRegular.name
    cursorDelegate: Item {
        visible: false
    }
}
