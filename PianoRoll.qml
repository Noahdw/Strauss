import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
PianoRollForm {
    property int pianoCols: 50
    property int pianoColsPos: pianoCols
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

                GridLayout{
                    id: pianoGrid
                    width: scrollBar.width;
                    height: pianoCol.height;
                    rows:132
                    columns: pianoCols
                    rowSpacing: 0;    columnSpacing: 0
                Repeater{

                model: pianoCols
                delegate: Repeater{

                        model: 132
                        property int rowIndex: index
                        delegate: Rectangle {
                            property int columnIndex: index
                            id: rectBar
                            width: scrollBar.width/ pianoCols; height: keyHeight
                            color: "light blue"
                            Layout.row: columnIndex
                            Layout.column: rowIndex
                            Component.onCompleted: {
                           if(index===130)
                               pianoColsPos-=1
                            }
                            border {
                                color: "grey"
                                width: 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onDoubleClicked: {
                                    rectBar.color = "dark blue"

                                }
                            }

                        }

                }
                }
                }
        }
    }
}
/*
                    Repeater{
                        model: 132

                        delegate: Rectangle{

                            width: scrollBar.width
                            height: keyHeight
                            color: "transparent"
                            border {
                                     color: "grey"
                                     width: 1
                                   }

                        }
                    }

  */
