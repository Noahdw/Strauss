import QtQuick 2.4

Rectangle {
    id:rect
    property int quad: 0
    property int id: 0
    property double scale: 0
    property double xchange: 0
    property double tscale: 0
    property int yCur: 0
    property int xCur: 0
    property int noteStart: 0
    property int noteEnd: 0
    property int note: 0
    property bool freeMove: false
    height: keyHeight
    width: noteEnd/tPQN*testSpace
    color: "light blue"
    x: testSpace * quad * scale +(width*xchange)
    y: keyHeight * note
    border { width: 1; color: "black" }


    MouseArea{
        id: mouseArea
        anchors.fill: parent
        drag.target: hiddenItem

        onClicked: {
            rect.color = "darkslateblue"
            activeId = id
        }
        onDoubleClicked:{
            rect.destroy()
        }
        onPressed: {
            activeId = id
          hiddenItem.y = mouse.y
            hiddenItem.x = mouse.x
            yCur = mouse.y
            //xCur = mouse.x
            freeMove = true

        }
        onReleased: {
        freeMove = false
        }
        Item{

            id:hiddenItem

            Drag.active: mouseArea.drag.active
            property int ypos: 0
            property int xpos: 0

            onYChanged: {
                ypos=y/keyHeight
                rect.y += keyHeight*ypos
            }
            onXChanged: {
                if(freeMove ===true){
                    var sign = x > 0 ? 1 : x == 0 ? 0 : -1
                    xchange+= .1 *sign
                    if(x< rect.width){

                        freeMove = false
                    }
                    else if(x>rect.width){
                        freeMove = false

                    }

                }
                else{
                xpos=x/(testSpace*scale)

                if(x< 0){
               xchange--
                }
                if(x>rect.width){
                 xchange++
                }


                }



                console.log(x)


            }
        }
    }
    Component.onCompleted: {
        activeId = id

    }

}
