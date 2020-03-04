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

            function getConnectedCameras(cameras) {
                var result = 0
                for (var i = 0; i < cameras.length; ++i) {
                    if (cameras[i].connected) {
                        result++;
                    }
                }
                return result
            }

            anchors {
                left: parent.left
                leftMargin: Config.Dimensions.smallMargin
                top: parent.top
                bottom: parent.bottom
            }

            verticalAlignment: Text.AlignVCenter

            text: qsTr("SDK Version %1 cameras: %2 connected cameras: %3").arg(CameraSDK.version).arg(CameraSDK.cameras.length).arg(getConnectedCameras(CameraSDK.cameras))

            color: Config.Theme.textPrimary
        }
    }
}
