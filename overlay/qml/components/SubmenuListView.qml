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

        border.color: Kirigami.Theme.backgroundColor
        border.width: 2
    }

    function getButtonName(model) 
    {
        if (model.action == "record-toggle") 
        {
            return !recording.isActive ? qsTr("Start") : qsTr("Stop")
        }
        return qsTr(model.name)
    }

    function getButtonIcon(model)
    {
        if (model.action == "record-toggle") 
        {
            return recording.isActive ? "media-playback-stop" : "media-playback-start"
        }
        return model.iconName
    }

    function getButtonEnabled(model)
    {
        if (model.action == "record-pause") 
        {
            return recording.isActive
        }
        return true;
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
                border.color: ddItem.hovered && getButtonEnabled(model) ? accentColor : 'transparent'
                border.width: 2
                radius: 3
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: getButtonEnabled(model)
                onClicked: {
                    if (!getButtonEnabled(model))
                    {
                        return;
                    } 
                    ddItem.clicked()
                }
                onEntered: ddItem.hoveredChanged()
                onExited: ddItem.hoveredChanged()
                cursorShape: getButtonEnabled(model) ? Qt.PointingHandCursor : Qt.ForbiddenCursor
            }

            contentItem: RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignVCenter

                Kirigami.Icon {
                    Layout.leftMargin: 8
                    source: getButtonIcon(model)
                    implicitWidth: 15
                    Layout.alignment: Qt.AlignVCenter
                    color: '#ffffff'
                    opacity: getButtonEnabled(model) ? 0.8 : 0.3
                }

                Text {
                    Layout.alignment: Qt.AlignVCenter
                    text: getButtonName(model)
                    color: 'white'
                    Layout.fillWidth: true
                    opacity: getButtonEnabled(model) ? 1 : 0.6
                }
            }
            
            onClicked: root.clicked(model.action, model.iconName, model.name)
        }
    }
    
    signal clicked(string action, string iconName, string name)
}