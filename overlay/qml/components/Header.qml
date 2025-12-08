import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

RowLayout {
    anchors.fill: parent
    Image {
        source: "../../images/gpu_screen_recorder_logo.png"
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
        id: closeButton
        Layout.rightMargin: 15
        icon.name: 'dialog-close'
        icon.width: 20
        icon.height: 20
        font.pointSize: 20
        background: Rectangle {
            color: 'transparent'
            border.color: closeButton.down ? '#a4333333' : (closeButton.hovered ? '#77969696': 0)
            radius: 8
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: closeButton.clicked()
            onEntered: closeButton.hoveredChanged()
            onExited: closeButton.hoveredChanged()
            cursorShape: Qt.PointingHandCursor
        }

        opacity: closeButton.down ? 0.5 : 1
        text: ''
        onClicked: {
            shutdownNotifier.intendedClose();
        }
    }
}