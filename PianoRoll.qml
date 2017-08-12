import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
PianoRollForm {
    property int canvasWidth: main.width
    property int canvasHeight: 100
     property int scrollXBinding: 0
    property int pianoCols: 15
    property int pianoColsPos: pianoCols
    property int scrollPos: scrollKeys.flickableItem.contentY
    property int scrollPos2: scrollBar.flickableItem.contentY
    id: pRollForm
    onScrollPosChanged: {
        scrollBar.flickableItem.contentY = scrollPos
        scrollKeys.flickableItem.contentY = scrollPos
    }
    onScrollPos2Changed: {
        scrollBar.flickableItem.contentY = scrollPos2
        scrollKeys.flickableItem.contentY = scrollPos2
    }
    Rectangle{
        border.color: "black"
        border.width: 1
        color:"transparent"
        height: 500
        width: main.width +keyWidth
        Row{
            ScrollView{
                id:scrollKeys
                height: 400
                width: keyWidth
                verticalScrollBarPolicy:   Qt.ScrollBarAlwaysOff


                Column {
                    id: pianoCol;

                    Repeater{
                        id: rep
                        model:11
                        delegate: PianoKeys{

                        }

                    }

                }
            }
            Column{

                ScrollView{

                    id: scrollBar
                    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                    verticalScrollBarPolicy:   Qt.ScrollBarAlwaysOff
                    width: main.width - keyWidth;
                    height: 400

                    DrawGrid{

                    }

                }  Rectangle{
                    id:velocityRect
                    height:canvasHeight
                    width:main.width - keyWidth
                    color: "grey"
                ScrollView{
                    flickableItem.contentX: scrollXBinding
                    height:velocityRect.height
                    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                    verticalScrollBarPolicy:   Qt.ScrollBarAlwaysOff

                    width:velocityRect.width

                        Canvas{
                            id:velocityCanvas
                            width:canvasWidth
                            height:velocityRect.height
                        }



                }
                }
            }
        }
    }

}
