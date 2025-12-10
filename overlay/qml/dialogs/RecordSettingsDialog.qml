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

    function outputToReadableText(output) 
    {
        if (output == "screen")
        {
            return "Focused monitor"
        }
        if (output == "portal")
        {
            return "Desktop Portal"
        }
        if (output == "region")
        {
            return "Rectangular area"
        }
        if (output.includes("|")) {
            const data = output.split("|")
            return "Monitor " + data[0] + " (" + data[1] + ")"
        }

        if (output && output.length > 0) {
            const options = recording.getCaptureOptions()

            for (let i = 0; i < options.length; i++) {
                if (options[i].startsWith(output + "|")) {
                    const res = options[i].split("|")[1]
                    return "Monitor " + output + " (" + res + ")"
                }
            }
            
            return "Monitor " + output
        }

        return "Unknown"
    }

    ColumnLayout {
        Item { Layout.minimumWidth: 500 }

        Label {
            text: qsTr('Capture method:')
        }
        ComboBox {
            id: captureOptionsCombo
            model: recording.getCaptureOptions()
            currentValue: settings.recordCaptureOption
            Layout.fillWidth: true

            displayText: outputToReadableText(currentValue)

            delegate: ItemDelegate {
                text: outputToReadableText(modelData)
                width: parent.width
                onClicked: {
                    settings.recordCaptureOption = modelData.split('|')[0]
                }
                required property string modelData
            }
        }

        Label {
            Layout.topMargin: 20
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