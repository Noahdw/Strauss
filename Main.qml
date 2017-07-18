import QtQuick 2.4
import QtQuick.Controls 1.4

PianoRollForm {
    id: main;
    signal playNote(string note,string offOn)
    property int globalKeys: 127
    property int pressed: 0
    property int keyWidth: 30
    property int keyHeight: 12
    width: 800; height: 800
    PianoRoll{
        anchors.bottom: parent.bottom;

    }
}
