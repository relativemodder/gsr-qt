import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import "." as Components

ColumnLayout {
    id: smallSideButtonsColumn

    implicitHeight: 220
    Item {
        Layout.fillHeight: true
    }
    Rectangle {
        color: '#98000000'
        Layout.leftMargin: 10
        implicitWidth: 70
        implicitHeight: 70
        radius: 5

        SystemPalette { id: activeSystemPalette; colorGroup: SystemPalette.Active }

        Components.SmallIconButton {
            anchors.fill: parent
            imageSize: 30
            buttonImageSource: '../../images/settings.png'
            accentColor: activeSystemPalette.accent

            onClicked: {
                smallSideButtonsColumn.settingsClicked();
            }
        }
    }

    signal settingsClicked()
}