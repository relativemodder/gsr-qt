import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import "." as Components

Rectangle {
    id: root
    
    color: '#98000000'
    Layout.fillWidth: true
    Layout.maximumWidth: 680
    implicitHeight: 220
    radius: 5

    // Signals
    signal buttonClicked(string buttonType)
    signal buttonHoveredWhenActive(string buttonType)

    // Button states
    property var activeStates: ({
        replay: false,
        record: false,
        stream: false
    })

    // Configuration data
    property var buttonConfigs: [
        { type: 'replay', active: false, mainText: qsTr('Instant Replay'), stateText: qsTr('Off'), imageSource: '../../images/replay.png' },
        { type: 'record', active: false, mainText: qsTr('Record'), stateText: qsTr('Not recording'), imageSource: '../../images/record.png' },
        { type: 'stream', active: false, mainText: qsTr('Livestream'), stateText: qsTr('Not streaming'), imageSource: '../../images/stream.png' }
    ]

    SystemPalette { 
        id: palette
        colorGroup: SystemPalette.Active 
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Repeater {
            model: root.buttonConfigs

            BigButton {
                required property var modelData
                
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.minimumHeight: 200
                
                mainText: modelData.mainText
                stateText: modelData.stateText
                buttonImageSource: modelData.imageSource
                accentColor: palette.accent
                stillHovered: root.activeStates[modelData.type] || false
                stillActive: modelData.active || false

                onHoveredChanged: {
                    if (hovered && (root.activeStates['replay'] || root.activeStates['record'] || root.activeStates['stream'])) {
                        root.buttonHoveredWhenActive(modelData.type)
                    }
                }
                
                onClicked: root.buttonClicked(modelData.type)
            }
        }
    }
}