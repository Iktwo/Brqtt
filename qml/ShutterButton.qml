import QtQuick 2.0
import config 1.0 as Config

Item {
    id: root

    signal triggered

    property alias count: rectangle.count

    height: Config.Dimensions.shutterButtonSize
    width: Config.Dimensions.shutterButtonSize

    Rectangle {
        id: rectangle

        property bool automated: false
        property int count: 100
        property int currentCount: 0

        anchors.centerIn: parent

        height: Config.Dimensions.shutterButtonSize
        width: Config.Dimensions.shutterButtonSize
        radius: Config.Dimensions.shutterButtonSize

        color: automated ? Config.Theme.automationStop : Config.Theme.automationStart

        border {
            color: Config.Theme.textSecondary
            width: Config.Dimensions.shutterButtonSize * 0.06
        }

        onAutomatedChanged: {
            if (automated === false) {
                timer.stop()
                currentCount = 0
            }
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent

            onClicked: {
                rectangle.automated = !rectangle.automated

                if (rectangle.automated) {
                    timerTrigger.start()
                }
            }
        }

        Timer {
            id: timerTrigger

            interval: 4000

            repeat: true
            triggeredOnStart: true

            onTriggered: {
                if (rectangle.currentCount == rectangle.count) {
                    rectangle.automated = false
                } else {
                    rectangle.currentCount += 1
                    root.triggered()
                }
            }
        }
    }
}
