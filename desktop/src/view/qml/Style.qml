import QtQuick 2.9

pragma Singleton

QtObject {

    function dim(color) {
        return Qt.tint(color, Qt.rgba(1, 1, 1, 1 - opacityDimmed));
    }

    function disable(color) {
        return Qt.tint(color, Qt.rgba(1, 1, 1, 1 - opacityDisabled));
    }

    function rgba(r, g, b, a) {
        return Qt.rgba(r/255., g/255., b/255., a);
    }

    readonly property real opacityDimmed: 0.85
    readonly property real opacityDisabled: 0.6

    readonly property color colorBackground: rgba(255, 255, 255, 1)
    readonly property color colorDisabled: rgba(211, 211, 211, 1)
    readonly property color colorAlert: rgba(50, 205, 50, 1)

    readonly property color colorPrimary: rgba(128, 0, 128, 1)
    readonly property color colorSecondary: rgba(224, 255, 255, 1)
    readonly property color colorTertiary: rgba(105, 105, 105, 1)

    readonly property color textColorNormal: rgba(105, 105, 105, 1)
    readonly property color textColorError: dim(colorPrimary)

    readonly property int radius: 3

    readonly property int fontSizeHuge: 18
    readonly property int fontSizeLarge: 14
    readonly property int fontSizeNormal: 12
    readonly property int fontSizeSmall: 10
    readonly property int fontSizeTiny: 8

    readonly property FontLoader fontBold: FontLoader {
        source: "qrc:/view/font/OpenSans-Bold.ttf"
    }

    readonly property FontLoader fontBoldItalic: FontLoader {
        source: "qrc:/view/font/OpenSans-BoldItalic.ttf"
    }

    readonly property FontLoader fontExtraBold: FontLoader {
        source: "qrc:/view/font/OpenSans-ExtraBold.ttf"
    }

    readonly property FontLoader fontExtraBoldItalic: FontLoader {
        source: "qrc:/view/font/OpenSans-ExtraBoldItalic.ttf"
    }

    readonly property FontLoader fontLight: FontLoader {
        source: "qrc:/view/font/OpenSans-Light.ttf"
    }

    readonly property FontLoader fontLightItalic: FontLoader {
        source: "qrc:/view/font/OpenSans-LightItalic.ttf"
    }

    readonly property FontLoader fontRegular: FontLoader {
        source: "qrc:/view/font/OpenSans-Regular.ttf"
    }

    readonly property FontLoader fontRegularItalic: FontLoader {
        source: "qrc:/view/font/OpenSans-RegularItalic.ttf"
    }

    readonly property FontLoader fontSemiBold: FontLoader {
        source: "qrc:/view/font/OpenSans-SemiBold.ttf"
    }

    readonly property FontLoader fontSemiBoldItalic: FontLoader {
        source: "qrc:/view/font/OpenSans-SemiBoldItalic.ttf"
    }

}
