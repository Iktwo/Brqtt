import QtQuick 2.14
import QtQuick.Controls 2.14
import com.iktwo.brqtt 1.0
import config 1.0 as Config

Item {
    height: 30

    Rectangle {
        anchors.fill: parent

        color: Config.Theme.controlBackground

        Label {
            id: labelSDKVersion

            anchors {
                left: parent.left
                leftMargin: Config.Dimensions.smallMargin
                top: parent.top
                bottom: parent.bottom
            }

            verticalAlignment: Text.AlignVCenter

            text: qsTr("SDK Version %1 camera connected: %2 live view state: %3")
            .arg(CameraSDK.version)
            .arg(CameraSDK.camera != null && CameraSDK.camera.connected)
            .arg(CameraSDK.camera == null ? qsTr("Disabled") : (CameraSDK.camera.liveViewState === BrqttCamera.Enabled ? qsTr("Enabled") : CameraSDK.camera.liveViewState === BrqttCamera.Disabled ? qsTr("Disabled") : "Not supported"))

            color: Config.Theme.textPrimary
        }
    }
}
