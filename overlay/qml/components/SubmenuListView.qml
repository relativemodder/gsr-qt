// components/SubmenuListView.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root
    
    property alias listModel: listView.model
    property bool menuVisible: false
    property var activeSystemPalette

    Layout.fillHeight: true
    width: 220
    
    color: 'transparent'
    clip: true

    Rectangle {
        id: bgRect
        clip: true
        color: '#000000'
        radius: 5
        visible: root.menuVisible
        
        width: 220
        anchors.fill: parent
    }

    ListView {
        id: listView
        
        width: 210
        anchors.fill: parent
        
        visible: root.menuVisible

        delegate: ItemDelegate {
            id: ddItem
            width: ListView.view.width
            height: 45
            
            readonly property color accentColor: activeSystemPalette ? activeSystemPalette.accent : Kirigami.Theme.highlightColor

            background: Rectangle {
                color: ddItem.down ? '#21ffffff' : 'transparent'
                border.color: ddItem.hovered ? accentColor : 'transparent'
                border.width: 2
                radius: 5

                Behavior on color { 
                    ColorAnimation { duration: 70 } 
                }

                Behavior on border.color { 
                    ColorAnimation { duration: 70 } 
                }
            }

            contentItem: RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignVCenter

                Kirigami.Icon {
                    Layout.leftMargin: 5
                    source: model.iconName
                    implicitWidth: 15
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    Layout.alignment: Qt.AlignVCenter
                    text: qsTr(model.name)
                    color: Kirigami.Theme.textColor
                    Layout.fillWidth: true
                }
            }
            
            onClicked: root.clicked(model.iconName, model.name)
        }
    }
    
    signal clicked(string iconName, string name)
}