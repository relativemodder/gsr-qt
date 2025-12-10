// BigButton.qml

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Button {
    id: bigButton
    
    property color accentColor: '#13e900'
    property bool stillActive: false
    property bool stillHovered: false
    property alias buttonImageSource: buttonImage.source
    property alias mainText: topText.text
    property alias stateText: bottomText.text
    property double imageSize: 100

    MouseArea {
        id: bigButtonArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: bigButton.clicked()
        onEntered: hoveredChanged()
        onExited: hoveredChanged()
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        color: bigButtonArea.containsPress ? '#232323' : (bigButton.hovered || bigButton.stillHovered ? 'black' : 'transparent')
        border.color: bigButton.hovered || bigButton.stillHovered ? bigButton.accentColor : 'transparent'
        border.width: 2
        radius: 5
    }

    contentItem: ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter 
        spacing: 5

        Text {
            id: topText
            text: 'Top text'
            color: 'white'
            font.bold: true
            font.pointSize: bottomText.font.pointSize + 1
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter     
            Layout.maximumWidth: bigButton.width
            Layout.minimumWidth: 1
            Layout.topMargin: 5
        }

        Item {
            width: bigButton.imageSize
            height: bigButton.imageSize
            Layout.alignment: Qt.AlignHCenter
            
            Image {
                id: buttonImage
                sourceSize: Qt.size(parent.width, parent.height)
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                smooth: true
            }

            MultiEffect {
                source: buttonImage
                anchors.fill: parent
                colorization: bigButton.stillActive ? 1 : 0
                colorizationColor: bigButton.accentColor
            }
        }
        
        Text {
            id: bottomText
            text: 'Bottom text'
            color: bigButton.stillActive ? bigButton.accentColor : '#a8a8a8'
            opacity: 0.5
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter     
            Layout.maximumWidth: bigButton.width
            Layout.minimumWidth: 1
            Layout.bottomMargin: 5
        }
    }

}