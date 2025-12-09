import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import org.kde.layershell as LayerShell
import org.kde.kirigami as Kirigami

Window {
    id: root
    visible: false
    color: "transparent"

    property string messageText: backendText
    property string iconName: backendIcon
    property int readSpeed: backendReadSpeed
    property string notificationType: backendNotificationType

    width: notificationText.paintedWidth + 150
    height: Math.max(120, notificationText.paintedHeight + 40)

    LayerShell.Window.anchors: LayerShell.Window.AnchorTop | LayerShell.Window.AnchorRight
    LayerShell.Window.margins.top: 80
    LayerShell.Window.exclusionZone: -1
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
    LayerShell.Window.keyboardInteractivity: LayerShell.Window.KeyboardInteractivityNone

    Component.onCompleted: {
        visible = true
        showSequence.start()
    }

    // root item gets animated
    Item {
        id: container
        anchors.fill: parent
        opacity: 0

        transform: Translate {
            id: slideTransform
            x: root.width
        }

        Rectangle {
            id: card
            anchors.fill: parent
            anchors.margins: 20
            anchors.rightMargin: 0
            topLeftRadius: 5
            bottomLeftRadius: 5
            color: Kirigami.Theme.backgroundColor
            clip: true

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    id: highlightBar
                    width: card.width
                    Layout.fillHeight: true
                    color: notificationType == 'error' ? Kirigami.Theme.negativeTextColor : Kirigami.Theme.highlightColor
                    topLeftRadius: 5
                    bottomLeftRadius: 5
                }

                Kirigami.Icon {
                    id: icon
                    source: iconName
                    width: 64
                    height: 64
                    opacity: 0
                    Layout.leftMargin: 30
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: notificationText
                    text: messageText
                    color: Kirigami.Theme.textColor
                    wrapMode: Text.WordWrap
                    Layout.alignment: Qt.AlignVCenter
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20
                    Layout.minimumWidth: 200
                    Layout.maximumWidth: 700
                    Layout.fillWidth: true
                    opacity: 0
                }

                Item { Layout.fillWidth: true }
            }
        }
    }

    // show animation
    SequentialAnimation {
        id: showSequence

        PauseAnimation { duration: 80 }

        NumberAnimation { target: container; property: "opacity"; to: 1; duration: 120 }

        NumberAnimation {
            target: slideTransform
            property: "x"
            to: 0
            duration: 160
            easing.type: Easing.OutCubic
        }

        PauseAnimation { duration: 40 }

        NumberAnimation {
            target: highlightBar
            property: "width"
            to: 3
            duration: 160
            easing.type: Easing.InOutQuad
        }

        ParallelAnimation {
            NumberAnimation { target: icon; property: "opacity"; to: 1; duration: 200 }
            NumberAnimation { target: notificationText; property: "opacity"; to: 1; duration: 200 }
        }

        PauseAnimation { duration: notificationText.text.length * readSpeed }

        ScriptAction { script: hideSequence.start() }
    }

    // hide animation
    SequentialAnimation {
        id: hideSequence

        ParallelAnimation {
            NumberAnimation { target: icon; property: "opacity"; to: 0; duration: 200 }
            NumberAnimation { target: notificationText; property: "opacity"; to: 0; duration: 200 }
        }

        NumberAnimation {
            target: highlightBar
            property: "width"
            to: card.width
            duration: 150
            easing.type: Easing.InOutQuad
        }

        PauseAnimation { duration: 50 }

        NumberAnimation {
            target: slideTransform
            property: "x"
            to: root.width
            duration: 160
            easing.type: Easing.InCubic
        }

        NumberAnimation { target: container; property: "opacity"; to: 0; duration: 120 }

        ScriptAction { script: Qt.quit() }
    }
}
