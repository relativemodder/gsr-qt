import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import "." as Components

Rectangle {
    id: mainButtonRowRect
    
    color: '#98000000'
    Layout.fillWidth: true
    Layout.maximumWidth: 680
    implicitHeight: 220
    radius: 5

    signal replayButtonClicked()
    signal recordButtonClicked()
    signal streamButtonClicked()

    property bool replayBtnActive: false
    property bool recordBtnActive: false
    property bool streamBtnActive: false

    RowLayout {
        id: mainButtonRow
        Layout.fillWidth: true
        anchors.fill: parent

        ListModel {
            id: mainButtonRowModel
            ListElement { cb: 'replay'; mainText: 'Instant Replay'; stateText: 'Off'; buttonImageSource: '../../images/replay.png' }
            ListElement { cb: 'record'; mainText: 'Record'; stateText: 'Not recording'; buttonImageSource: '../../images/record.png' }
            ListElement { cb: 'stream'; mainText: 'Livestream'; stateText: 'Not streaming'; buttonImageSource: '../../images/stream.png' }
        }
        
        SystemPalette { id: activeSystemPalette; colorGroup: SystemPalette.Active }

        Repeater {
            model: mainButtonRowModel

            BigButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 200
                Layout.minimumHeight: 200
                mainText: model.mainText
                stateText: model.stateText
                buttonImageSource: model.buttonImageSource
                accentColor: activeSystemPalette.accent
                
                stillHovered: model.cb == 'replay' ? mainButtonRowRect.replayBtnActive :
                    model.cb == 'record' ? mainButtonRowRect.recordBtnActive :
                    model.cb == 'stream' ? mainButtonRowRect.streamBtnActive :
                                            false

                onClicked: {
                    mainButtonRowRect[model.cb + 'ButtonClicked']()
                }
            }
        }
    }
}