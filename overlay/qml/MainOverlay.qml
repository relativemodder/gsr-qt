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
    title: 'test'

    LayerShell.Window.anchors: LayerShell.Window.AnchorTop
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
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
        ListElement { iconName: "media-playback-start"; name: "Turn On" }
        ListElement { iconName: "document-save"; name: "Save" }
        ListElement { iconName: "document-save"; name: "Save 1 min" }
        ListElement { iconName: "document-save"; name: "Save 10 min" }
        ListElement { iconName: "settings-configure"; name: "Settings" }
    }
    
    ListModel {
        id: recordListModel
        ListElement { iconName: "media-playback-start"; name: "Start" }
        ListElement { iconName: "media-playback-pause"; name: "Pause" }
        ListElement { iconName: "settings-configure"; name: "Settings" }
    }
    
    ListModel {
        id: streamListModel
        ListElement { iconName: "media-playback-start"; name: "Start" }
        ListElement { iconName: "settings-configure"; name: "Settings" }
    }

    // Dimmer overlay
    Rectangle {
        id: dimmer
        anchors.fill: parent
        color: '#83000000'
        opacity: 0
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

                Components.SubmenuListView {
                    Layout.minimumHeight: 230
                    listModel: replayListModel
                    menuVisible: submenuState.replay
                    activeSystemPalette: palette
                    onClicked: (icon, name) => submenuState.replay = false
                }

                Item { width: 0 } 
                
                Components.SubmenuListView {
                    Layout.maximumHeight: 140
                    Layout.alignment: Qt.AlignTop
                    listModel: recordListModel
                    menuVisible: submenuState.record
                    activeSystemPalette: palette
                    onClicked: (icon, name) => submenuState.record = false
                }

                Components.SubmenuListView {
                    Layout.maximumHeight: 95
                    Layout.alignment: Qt.AlignTop
                    listModel: streamListModel
                    menuVisible: submenuState.stream
                    activeSystemPalette: palette
                    onClicked: (icon, name) => submenuState.stream = false
                }

                Item { Layout.fillWidth: true }
            }
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

        PauseAnimation { duration: 100 }

        ParallelAnimation {
            NumberAnimation {
                target: dimmer
                property: "opacity"
                to: 1
                duration: 300
                easing.type: Easing.OutQuad
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

        PauseAnimation { duration: 100 }

        ParallelAnimation {
            NumberAnimation {
                target: dimmer
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