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

        onCameraChanged: {
            if (CameraSDK.camera != null) {
                CameraSDK.camera.connectToDevice()
            }
        }
    }

    Label {
        id: labelAperture
        anchors.fill: parent
        color: "Yellow"
        visible: text != "F0"
        text: "F" + (CameraSDK.camera === null ? 0 : CameraSDK.camera.aperture)
    }

    Connections {
        target: CameraSDK.camera

        enabled: CameraSDK.camera != null
    }

    MessageOverlay {
        id: messageOverlay

        parent: Overlay.overlay
    }
}
