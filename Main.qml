import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

PianoRollForm {
    id: main;
    signal playNote(string note,string offOn)
    signal addNotes()
    signal playSong()
    signal updateMidi(int note, int start, int length)
    property int globalKeys: 127
    property int pressed: 0
    property int keyWidth: 30
    property int keyHeight: 12
    property int testSpace: 132*12/totalCols // Keyheight*numkeys/cols. Initial value. Will be overriden.
    property int activeId: -1
    property int tPQN: 120
    property int totalCols: 4*20 //Means 20 whole notes
    property double scaleFactor: 1
    width: Screen.width ; height: 600

    PianoRoll{
        anchors.bottom: parent.bottom;

    }
    Rectangle{
        id: loadRect
        height: 100
        color:"grey"
        width:100
        anchors.top: parent.top
        border{
            width:1
            color:"black"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                main.addNotes()

            }
        }
    }
    Rectangle{
        id: playRect
        height: 100
        color:"grey"
        width:100
        border{
            width:1
            color:"black"
        }
        anchors.top: parent.top
        anchors.left: loadRect.right
        MouseArea{
            anchors.fill: parent
            onClicked: {
                main.playSong()

            }
        }
    }

}
