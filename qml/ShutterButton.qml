import QtQuick 2.0
import config 1.0 as Config

Item {
    id: root

    signal triggered

    height: Config.Dimensions.shutterButtonSize
    width: Config.Dimensions.shutterButtonSize

    Rectangle {
        anchors.centerIn: parent

        height: Config.Dimensions.shutterButtonSize
        width: Config.Dimensions.shutterButtonSize
        radius: Config.Dimensions.shutterButtonSize

        color: Config.Theme.textPrimary

        border {
            color: Config.Theme.textSecondary
            width: Config.Dimensions.shutterButtonSize * 0.06
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent

            onClicked: root.triggered()
        }
    }
}
