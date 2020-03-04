import QtQuick 2.14
import QtQuick.Controls 2.14
import com.iktwo.brqtt 1.0
import config 1.0 as Config
import Qt.labs.settings 1.0

ApplicationWindow {
    id: rootWindow

    visible: true
    width: 1280
    height: 720
    title: qsTr("Brqtt")

    color: Config.Theme.windowBackground

    footer: InformationBar {
        id: informationBar
    }

    // TODO: Verify if there is a way to have minimum values and if it is possible to restore X, Y
    Settings {
        property alias x: rootWindow.x
        property alias y: rootWindow.y
        property alias width: rootWindow.width
        property alias height: rootWindow.height
    }

    Connections {
        target: CameraSDK

        onFailedToInitialize: {
            messageOverlay.showMessage("Failed to initialize SDK")
        }

        onCamerasChanged: {
            // If there is a single camera detected, let's try to connect to it
            if (CameraSDK.cameras.length === 1) {
                CameraSDK.cameras[0].connect()
            }
        }
    }

    ListView {
        id: listViewCameraSelection

        anchors.fill: parent

        delegate: Button {
            text: modelData.model

            onClicked: modelData.connect()

            Rectangle {
                anchors.fill: parent
                opacity: 0.4
                color: modelData.connected ? "green" : "red"
            }
        }

        model: CameraSDK.cameras
    }

    MessageOverlay {
        id: messageOverlay

        parent: Overlay.overlay
    }

    Component.onCompleted: CameraSDK.initializeSDK()
}
