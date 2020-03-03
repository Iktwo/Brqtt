import QtQuick 2.0
import QtQuick.Controls 2.14

Item {
    function showMessage(message) {
        label.text = message
        visible = true
    }

    visible: false
    height: rootWindow.height
    width: rootWindow.width

    Rectangle {
        height: rootWindow.height
        width: rootWindow.width

        Label {
            id: label

            anchors.fill: parent
        }
    }
}
