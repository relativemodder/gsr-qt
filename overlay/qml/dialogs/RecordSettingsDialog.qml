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
            text: 'Output video directory:'
        }
        RowLayout {
            Layout.fillWidth: true
            TextField {
                id: outputVideoDirTF
                Layout.fillWidth: true
                text: settings.recordOutputDirectory
            }
            Button {
                text: 'Save'
                Layout.leftMargin: 5
                onClicked: {
                    settings.recordOutputDirectory = outputVideoDirTF.text
                }
            }
        }

        Switch {
            text: 'Save video in a folder with the name of the game (X11 and KDE Plasma only)'
            leftPadding: 0
            Layout.topMargin: 10
            checked: settings.recordCategorizeByTitle
            onClicked: settings.recordCategorizeByTitle = checked
        }
    }
}