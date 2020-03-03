import QtQuick 2.14
import QtQuick.Controls 2.14
import com.iktwo.brqtt 1.0
import config 1.0 as Config

ApplicationWindow {
    id: rootWindow

    visible: true
    width: 640
    height: 480
    title: qsTr("Brqtt")

    color: Config.Theme.windowBackground

    Connections {
        target: CameraSDK

        onFailedToInitialize: {
            messageOverlay.showMessage("Failed to initialize SDK")
        }
    }

    InformationBar {
        id: informationBar
    }

    MessageOverlay {
        id: messageOverlay
    }

    Component.onCompleted: CameraSDK.initializeSDK()
}
