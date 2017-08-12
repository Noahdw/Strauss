import QtQuick 2.4

Rectangle{
    property int dT: 0
    property int note: 0
    property int velocity: height
    property int xPos: 0
    property int canvasBinding: canvasWidth

    id:vBar
    width:2
    color:"black"
    height:50
    x: xPos
    y:canvasHeight-height


    onDTChanged: {
        var totDT = tPQN * totalCols
        var OldRange = (totDT - 0)
        var NewRange = (canvasWidth - 0)
        var NewValue = (((dT - 0) * NewRange) / OldRange) + 1

        xPos = Math.floor(NewValue)
    }
    onCanvasBindingChanged: {
        var totDT = tPQN * totalCols
        var OldRange = (totDT - 0)
        var NewRange = (canvasWidth - 0)
        var NewValue = (((dT - 0) * NewRange) / OldRange) + 1

        xPos = Math.floor(NewValue)
    }
    onVelocityChanged: {

        var OldRange = (127 - 1)
        var NewRange = (canvasHeight - 0)
        var NewValue = (((velocity - 1) * NewRange) / OldRange) + 0

        height = Math.floor(NewValue)
    }
    property int mouseLast: 0
    property bool dragable: false
    MouseArea{
        id:mouseArea
        anchors.fill: parent
        onMouseYChanged: {
            if(dragable){
                console.log(mouse.y)
             vBar.height +=1
                mouseLast = mouse.y
            }


        }
        onPressed: {
            if(mouse.y<10){
            dragable = true
            mouseLast = mouse.y
            }
        }
        onReleased: {
        dragable = false
        }
    }


}
