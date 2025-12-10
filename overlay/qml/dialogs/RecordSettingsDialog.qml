import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Dialog {
    id: scrollableDialog
    title: qsTr("Recording settings")
    maximumHeight: 500

    padding: 20

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AlwaysOn
    }

    ColumnLayout {
        Item { Layout.minimumWidth: 500 }

        Label {
            text: qsTr('Output video directory:')
        }
        RowLayout {
            Layout.fillWidth: true
            TextField {
                id: outputVideoDirTF
                Layout.fillWidth: true
                text: settings.recordOutputDirectory
                enabled: false
            }
            Button {
                text: outputVideoDirTF.enabled ? qsTr('Save') : qsTr('Edit')
                Layout.leftMargin: 5
                onClicked: {
                    outputVideoDirTF.enabled = !outputVideoDirTF.enabled
                    settings.recordOutputDirectory = outputVideoDirTF.text
                }
            }
        }

        Switch {
            text: qsTr('Save video in a folder with the name of the game')
            leftPadding: 0
            Layout.topMargin: 20
            checked: settings.recordCategorizeByTitle
            onClicked: settings.recordCategorizeByTitle = checked
        }
    }
}