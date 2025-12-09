import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Dialogs
import "./components" as Components
import org.kde.kirigami as Kirigami
import org.kde.layershell 1.0 as LayerShell

ApplicationWindow {
    id: root

    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    flags: Qt.Window | Qt.FramelessWindowHint
    color: 'transparent'
    visible: false
    title: 'GPU Screen Recorder Overlay'

    LayerShell.Window.anchors: LayerShell.Window.AnchorTop
    LayerShell.Window.layer: LayerShell.Window.LayerTop
    LayerShell.Window.exclusionZone: -1

    // Submenu state management
    QtObject {
        id: submenuState
        property bool replay: false
        property bool record: false
        property bool stream: false
        
        readonly property bool anyVisible: replay || record || stream
    }

    SystemPalette { 
        id: palette
        colorGroup: SystemPalette.Active 
    }

    // Methods
    function setSubmenuVisible(target, visible) {
        submenuState.replay = (target === 'replay' && visible)
        submenuState.record = (target === 'record' && visible)
        submenuState.stream = (target === 'stream' && visible)
    }

    function toggleSubmenu(target) {
        setSubmenuVisible(target, !submenuState[target])
    }

    function closeAllSubmenus() {
        submenuState.replay = false
        submenuState.record = false
        submenuState.stream = false
    }

    // Keyboard shortcuts
    Shortcut {
        sequence: "Esc"
        onActivated: {
            if (submenuState.anyVisible) {
                root.closeAllSubmenus()
            } else {
                shutdownNotifier.intendedClose()
            }
        }
    }

    // Data models
    ListModel {
        id: replayListModel
        ListElement {action: "replay-toggle"; iconName: "media-playback-start"; name: "Turn On" }
        ListElement {action: "replay-save"; iconName: "document-save"; name: "Save" }
        ListElement {action: "replay-save-1-min"; iconName: "document-save"; name: "Save 1 min" }
        ListElement {action: "replay-save-10-min"; iconName: "document-save"; name: "Save 10 min" }
        ListElement {action: "replay-settings"; iconName: "settings-configure"; name: "Settings" }
    }
    
    ListModel {
        id: recordListModel
        ListElement {action: "record-toggle"; iconName: "media-playback-start"; name: "Start" }
        ListElement {action: "record-pause"; iconName: "media-playback-pause"; name: "Pause" }
        ListElement {action: "record-settings"; iconName: "settings-configure"; name: "Settings" }
    }
    
    ListModel {
        id: streamListModel
        ListElement {action: "stream-toggle"; iconName: "media-playback-start"; name: "Start" }
        ListElement {action: "stream-settings"; iconName: "settings-configure"; name: "Settings" }
    }

    // Dimmer overlay
    Rectangle {
        id: dimmer
        anchors.fill: parent
        color: '#83000000'
        opacity: 0
        gradient: Gradient {
            GradientStop { position: 0.0; color: '#ea000000' }
            GradientStop { position: 1.0; color: '#5d000000' }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (submenuState.anyVisible) {
                    root.closeAllSubmenus()
                }
            }
        }
    }

    // Main panel
    Item {
        id: panelContainer
        y: -panelLayout.height
        width: parent.width
        height: panelLayout.implicitHeight 

        ColumnLayout {
            id: panelLayout
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            
            Component.onCompleted: panelContainer.height = implicitHeight

            // Header
            Rectangle {
                color: 'black'
                Layout.fillWidth: true
                height: 70
                
                Components.Header {}
            }
            
            // Main buttons
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 100

                Item { Layout.minimumWidth: 70 }

                Components.MainButtonRow {
                    id: mainButtonRow

                    activeStates: ({
                        replay: submenuState.replay,
                        record: submenuState.record,
                        stream: submenuState.stream
                    })

                    onButtonClicked: (type) => root.toggleSubmenu(type)
                    onButtonHoveredWhenActive: (type) => root.setSubmenuVisible(type, true)
                }
                
                Components.SmallSideButtonsColumn {}
            }

            // Submenus
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter 
                Layout.maximumWidth: 760

                Item { Layout.minimumWidth: 33 } // Why tf 35 you may ask? The upper buttons have 70px margins, so half of that.

                Components.SubmenuListView {
                    Layout.minimumHeight: 227
                    listModel: replayListModel
                    menuVisible: submenuState.replay
                    activeSystemPalette: palette
                    onClicked: (action, icon, name) => submenuState.replay = false
                }

                Item { width: 0 } 
                
                Components.SubmenuListView {
                    Layout.maximumHeight: 137
                    Layout.alignment: Qt.AlignTop
                    listModel: recordListModel
                    menuVisible: submenuState.record
                    activeSystemPalette: palette
                    onClicked: (action, icon, name) => {
                        if (action == 'record-toggle') {
                            recording.toggleRecording(!recording.isActive)
                        }
                        submenuState.record = false
                    }
                }

                Components.SubmenuListView {
                    Layout.maximumHeight: 92
                    Layout.alignment: Qt.AlignTop
                    listModel: streamListModel
                    menuVisible: submenuState.stream
                    activeSystemPalette: palette
                    onClicked: (action, icon, name) => submenuState.stream = false
                }

                Item { Layout.fillWidth: true }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    RowLayout {
        id: activeWindowContainer
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
        Layout.maximumHeight: 120

        MouseArea {
            id: activeWindowMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                activeWindowBackgroundRect.color = 'black'
            }
            onExited: {
                activeWindowBackgroundRect.color = '#c5000000'
            }
        }

        Rectangle {
            id: activeWindowBackgroundRect
            anchors.fill: parent
            color: '#c5000000'
            Layout.minimumHeight: 80
            radius: 10

            Behavior on color {
                ColorAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Text {
            id: activeWindowTitle
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            text: activeWindow.title || qsTr("No Active Window")
            color: 'white'
            font.pixelSize: 20
            leftPadding: 40
            rightPadding: 40
        }
    }

    // Animations
    Component.onCompleted: {
        if (panelContainer.height > 0) {
            openAnimation.start()
        }
    }

    Connections {
        target: shutdownNotifier
        function onStartShutdownAnimation() {
            closeAnimation.start()
        }
    }

    SequentialAnimation {
        id: openAnimation

        ParallelAnimation {

            SequentialAnimation {

                PauseAnimation { duration: 300 }
                NumberAnimation {
                    target: dimmer
                    property: "opacity"
                    to: 1
                    duration: 300
                    easing.type: Easing.OutQuad
                }
            }

            PropertyAnimation {
                target: panelContainer
                property: "y"
                to: 0
                duration: 400
                easing.type: Easing.OutCubic
            }
        }
    }

    SequentialAnimation {
        id: closeAnimation

        // PauseAnimation { duration: 100 }

        ParallelAnimation {
            NumberAnimation {
                target: dimmer
                property: "opacity"
                to: 0
                duration: 300
                easing.type: Easing.OutQuad
            }

            NumberAnimation {
                target: activeWindowContainer
                property: "opacity"
                to: 0
                duration: 300
                easing.type: Easing.OutQuad
            }

            PropertyAnimation {
                target: panelContainer
                property: "y"
                to: -panelLayout.height
                duration: 400
                easing.type: Easing.OutCubic
            }

            PropertyAnimation {
                target: panelContainer
                property: "opacity"
                to: 0
                duration: 400
                easing.type: Easing.OutCubic
            }
        }
    }
}