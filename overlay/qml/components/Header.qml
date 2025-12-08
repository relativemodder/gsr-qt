import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

RowLayout {
    anchors.fill: parent
    Image {
        source: "qrc:/GsrQt/images/gpu_screen_recorder_logo.png"
        Layout.maximumHeight: 40
        Layout.maximumWidth: 40
        fillMode: Image.PreserveAspectFit
        Layout.leftMargin: 15
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