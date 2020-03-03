import QtQuick 2.14
import QtQuick.Controls 2.14
import com.iktwo.brqtt 1.0
import config 1.0 as Config

Item {
    anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
    }

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

            text: qsTr("SDK Version %1").arg(CameraSDK.version)

            color: Config.Theme.textPrimary
        }
    }
}
