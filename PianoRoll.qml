import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
PianoRollForm {
    property int pianoCols: 15
    property int pianoColsPos: pianoCols
    id: pRollForm

    ScrollView{
        id: scrollBar
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOn
        verticalScrollBarPolicy:   Qt.ScrollBarAlwaysOn
        width: main.width;
        height: 400

        Row{

            Column {
                id: pianoCol;

                Repeater{
                    id: rep
                    model:11
                    delegate: PianoKeys{

                    }

                }

            }
            DrawGrid{


            }

        }
    }
}
