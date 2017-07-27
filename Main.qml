import QtQuick 2.4
import QtQuick.Controls 1.4

PianoRollForm {
    id: main;
    signal playNote(string note,string offOn)
    signal addNotes()
    property int globalKeys: 127
    property int pressed: 0
    property int keyWidth: 30
    property int keyHeight: 12
    property int testSpace: 132*12/totalCols // Keyheight*numkeys/cols. Initial value. Will be overriden.
    property int activeId: -1
    property int totalCols: 4*20 //Means 20 whole notes
    property double scaleFactor: 1
    width: 800; height: 800

    PianoRoll{
        anchors.bottom: parent.bottom;

    }
    Rectangle{
        height: 100
        color:"black"
        width:100
        anchors.top: parent.top
        MouseArea{
            anchors.fill: parent
            onClicked: {
                var values = manager.getNoteInfo();
                console.log("test:" + values[0] + values.length)
                main.addNotes()

            }
        }
    }

}
