import QtQuick 2.14
import QtQuick.Controls 2.14
import com.iktwo.brqtt 1.0
import config 1.0 as Config

Item {
    id: root

    property alias value: slider.precisionValue

    width: Config.Dimensions.sliderWidth
    height: childrenRect.height

    Slider {
        id: slider

        property var precisionValue: BrqttCamera.MediumPrecision

        width: Config.Dimensions.sliderWidth

        from: 1
        to: 3

        value: 2

        stepSize: 1

        snapMode: Slider.SnapOnRelease

        onValueChanged: {
            switch(value) {
            case 1:
                precisionValue = BrqttCamera.SmallPrecision
                break
            case 2:
                precisionValue = BrqttCamera.MediumPrecision
                break
            case 3:
                precisionValue = BrqttCamera.LargePrecision
                break
            }
        }
    }
}
