import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects

RowLayout {
    anchors.fill: parent
    Image {
        id: logoImage
        source: "qrc:/GsrQt/images/gpu_screen_recorder_logo.png"
        Layout.maximumHeight: 40
        Layout.maximumWidth: 40
        fillMode: Image.PreserveAspectFit
        Layout.leftMargin: 15

        layer.enabled: true
        layer.effect: MultiEffect {
            brightness: logoImageMouseArea.containsMouse ? 0.3 : 0.0
            Behavior on brightness {
                NumberAnimation { 
                    duration: 100
                    easing.type: Easing.OutQuad
                }
            }
        }

        ToolTip.visible: logoImageMouseArea.containsMouse
        ToolTip.text: qsTr("About GPU Screen Recorder")
        ToolTip.delay: 500

        opacity: logoImageMouseArea.containsPress ? 0.8 : 1.0

        MouseArea {
            id: logoImageMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: {
                Qt.openUrlExternally("https://git.dec05eba.com/gpu-screen-recorder/about/")
                shutdownNotifier.intendedClose();
            }
        }
    }
    Item {
        Layout.fillWidth: true
    }
    Text {
        text: 'GPU Screen Recorder'
        color: 'white'
        font.pointSize: 18
    }
    Item {
        Layout.fillWidth: true
    }
    Button {
        property bool pressedState: false

        id: closeButton
        Layout.rightMargin: 15
        icon.source: "qrc:/GsrQt/images/close.svg"
        icon.width: 24
        icon.height: 24
        text: ''
        font.pointSize: 24
        background: Rectangle {
            id: closeButtonBackground
            color: closeButton.pressedState ? '#a4ff0000' : 'transparent'
            border.color: closeButton.pressedState ? '#a4ff0000' : (closeButton.hovered ? '#77969696': 0)
            radius: 8
        }

        onClicked: {
            shutdownNotifier.intendedClose();
        }

        ToolTip.visible: closeButton.hovered
        ToolTip.text: qsTr("Close")
        ToolTip.delay: 500

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                closeButton.pressedState = true
            }
            onReleased: {
                closeButton.pressedState = false
            }
            onClicked: closeButton.clicked()
            onEntered: closeButton.hoveredChanged()
            onExited: closeButton.hoveredChanged()
            cursorShape: Qt.PointingHandCursor
        }
    }
}