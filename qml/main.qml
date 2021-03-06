import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Styles 1.4
import QtMultimedia 5.12
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

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Tools")

            Action {
                text: qsTr("&Detect cameras")

                onTriggered: CameraSDK.retrieveCameras()
            }

            MenuSeparator { }

            Action {
                text: qsTr("&Quit")

                onTriggered: Qt.quit()
            }
        }
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

    Connections {
        target: CameraSDK.camera === null ? null : CameraSDK.camera

        onConnectedChanged: {
            if (connected) {
                videoOutput.source = CameraSDK.camera
            }
        }
    }

    Item {
        anchors.fill: parent

        VideoOutput {
            id: videoOutput

            anchors.fill: parent

            onSourceRectChanged: console.log("sourceRect", sourceRect)
            onContentRectChanged: console.log("contentRect", contentRect)
        }

        Label {
            id: labelAspectRatio

            function formatAspectRatio(aspectRatio) {
                switch (aspectRatio) {
                case BrqttCamera.Ratio3_2:
                    return "3:2"

                case BrqttCamera.Ratio16_9:
                    return "16:9"

                case BrqttCamera.Ratio4_3:
                    return "4:3"

                case BrqttCamera.Ratio1_1:
                    return "1:1"
                }

                return ""
            }

            color: Config.Theme.textPrimary

            font.pointSize: 22

            text: CameraSDK.camera === null ? "" : formatAspectRatio(CameraSDK.camera.aspectRatio)
        }

        Label {
            id: labelAperture

            anchors {
                bottom: parent.bottom
                left: parent.left
            }

            color: Config.Theme.textPrimary

            font.pointSize: 22

            text: "F" + (CameraSDK.camera === null ? 0 : CameraSDK.camera.aperture.toFixed(1))

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    CameraSDK.camera.getLiveView()
                }
            }
        }

        ShutterButton {
            id: shutterButton

            count: spinBox.value

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            onTriggered: {
                if (checkBoxFar.checked) {
                    var focused = CameraSDK.camera.setFocusFar(focusPrecisionSlider.value)

                    if (checkBoxPhotoAfterFocus.checked && focused) {
                        CameraSDK.camera.takePhoto()
                    }
                } else {
                    var focused = CameraSDK.camera.setFocusFar(focusPrecisionSlider.value)

                    if (checkBoxPhotoAfterFocus.checked && focused) {
                        CameraSDK.camera.takePhoto()
                    }
                }
            }
        }

        Row {
            id: rowDirection

            anchors {
                right: parent.right
                bottom: rowFocus.top
            }

            SpinBox {
                id: spinBox

                from: 1
                to: 200

                value: 5
            }

            CheckBox {
                id: checkBoxFar

                text: "Far"
            }
        }

        Row {
            id: rowFocus

            anchors {
                right: parent.right
                bottom: parent.bottom
            }

            Button {
                id: buttonNear

                text: "Near"

                onClicked: {
                    var focused = CameraSDK.camera.setFocusNear(focusPrecisionSlider.value)

                    if (checkBoxPhotoAfterFocus.checked && focused) {
                        CameraSDK.camera.takePhoto()
                    }
                }
            }

            Button {
                id: buttonFar

                text: "Far"

                onClicked: {
                    var focused = CameraSDK.camera.setFocusFar(focusPrecisionSlider.value)

                    if (checkBoxPhotoAfterFocus.checked && focused) {
                        CameraSDK.camera.takePhoto()
                    }
                }
            }

            CheckBox {
                id: checkBoxPhotoAfterFocus

                checked: false

                text: "Photo after focusing"
                /// TODO: customize style
            }
        }

        FocusPrecisionSlider {
            id: focusPrecisionSlider

            anchors {
                right: parent.right
            }
        }
    }

    Connections {
        target: CameraSDK.camera

        enabled: CameraSDK.camera != null
    }

    MessageOverlay {
        id: messageOverlay

        parent: Overlay.overlay
    }

    Component.onCompleted: {
        if (CameraSDK.camera != null) {
            CameraSDK.camera.connectToDevice()
        }
    }
}
