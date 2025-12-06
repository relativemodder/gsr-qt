import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Button {
    id: smallIconButton

    property color accentColor: '#13e900'
    property bool stillActive: false
    property double imageSize: 40
    property alias buttonImageSource: buttonImage.source

    background: Rectangle {
        color: smallIconButton.hovered ? 'black' : 'transparent'
        border.color: smallIconButton.hovered ? smallIconButton.accentColor : 'transparent'
        border.width: 2
        radius: 5

        Behavior on color { 
            ColorAnimation { duration: 80 } 
        }

        Behavior on border.color { 
            ColorAnimation { duration: 80 } 
        }
    }

    contentItem:
        ColumnLayout {
            anchors.fill: parent
            anchors.centerIn: parent
            Layout.fillWidth: true

            Image {
                id: buttonImage
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                sourceSize: Qt.size(parent.width, parent.height)
                Layout.maximumWidth: smallIconButton.imageSize
                Layout.maximumHeight: smallIconButton.imageSize

                Layout.margins: 20
                fillMode: Image.PreserveAspectFit
                smooth: true
            }
        }
}