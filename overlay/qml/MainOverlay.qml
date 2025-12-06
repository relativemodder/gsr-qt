import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import "./components" as Components // Убедитесь, что этот путь верен
import org.kde.kirigami as Kirigami
import org.kde.layershell 1.0 as LayerShell

ApplicationWindow {
    id: mainWindow

    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    flags: Qt.Window | Qt.FramelessWindowHint
    color: 'transparent';
    visible: true
    title: 'test'

    LayerShell.Window.anchors: LayerShell.Window.AnchorTop
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
    LayerShell.Window.exclusionZone: -1

    Rectangle {
        id: dimmerRectangle
        anchors.fill: parent
        color: '#83000000'
        opacity: 0
    }

    SystemPalette { id: activeSystemPalette; colorGroup: SystemPalette.Active }

    property bool replaySubmenuVisible: false
    property bool recordSubmenuVisible: false
    property bool streamSubmenuVisible: false

    function setSubmenuVisible(target) {
        replaySubmenuVisible = (target === 'replay' && !replaySubmenuVisible)
        recordSubmenuVisible = (target === 'record' && !recordSubmenuVisible)
        streamSubmenuVisible = (target === 'stream' && !streamSubmenuVisible)
    }

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

    Item {
        id: panelContainer
        y: -panelLayout.height
        width: parent.width
        height: panelLayout.implicitHeight 

        ColumnLayout {
            id: panelLayout
            Layout.fillWidth: true
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            
            Component.onCompleted: panelContainer.height = implicitHeight

            // HEADER
            Rectangle {
                id: headerRect
                color: 'black'
                Layout.fillWidth: true
                height: 70
                
                Components.Header {}
            }
            
            // MAIN BUTTONS ROW
            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 100

                Components.MainButtonRow {
                    id: mainButtonRow

                    replayBtnActive: mainWindow.replaySubmenuVisible
                    recordBtnActive: mainWindow.recordSubmenuVisible
                    streamBtnActive: mainWindow.streamSubmenuVisible

                    onReplayButtonClicked: {
                        mainWindow.setSubmenuVisible('replay')
                    }

                    onRecordButtonClicked: {
                        mainWindow.setSubmenuVisible('record')
                    }

                    onStreamButtonClicked: {
                        mainWindow.setSubmenuVisible('stream')
                    }
                }
                Components.SmallSideButtonsColumn {}
            }

            // SUBMENUS (pls don't murder me)

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter 
                Layout.maximumWidth: 760

                Components.SubmenuListView {
                    Layout.minimumHeight: 230
                    listModel: replayListModel
                    menuVisible: mainWindow.replaySubmenuVisible
                    activeSystemPalette: mainWindow.activeSystemPalette
                    onClicked: (icon, name) => {
                        mainWindow.replaySubmenuVisible = false
                    }
                }

                Item { width: 0; } 
                
                Components.SubmenuListView {
                    Layout.maximumHeight: 140
                    Layout.alignment: Qt.AlignTop
                    listModel: recordListModel
                    menuVisible: mainWindow.recordSubmenuVisible
                    activeSystemPalette: mainWindow.activeSystemPalette
                    onClicked: (icon, name) => {
                        mainWindow.recordSubmenuVisible = false
                    }
                }

                Components.SubmenuListView {
                    Layout.maximumHeight: 95
                    Layout.alignment: Qt.AlignTop
                    listModel: streamListModel
                    menuVisible: mainWindow.streamSubmenuVisible
                    activeSystemPalette: mainWindow.activeSystemPalette
                    onClicked: (icon, name) => {
                        mainWindow.streamSubmenuVisible = false
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }
    }

    
    Component.onCompleted: {
        if (panelContainer.height > 0) {
            fullAnimation.start()
        }
    }

    Connections {
        target: shutdownNotifier
        
        function onStartShutdownAnimation() {
            closeAnimation.start()
        }
    }

    SequentialAnimation {
        id: fullAnimation

        PauseAnimation { duration: 100 }

        ParallelAnimation {
            NumberAnimation {
                target: dimmerRectangle
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
                target: dimmerRectangle
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