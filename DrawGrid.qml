import QtQuick 2.0
import QtQml 2.2

Canvas{
    id:canvas2
    width:canvas.width
    height:canvas.height
    property bool drewHorizLines: false
    property bool drewVerticals: false
    property int rows: 132
    property int columns: totalCols
    property int totIds: 0
    property int wheelZoom: 0
    property int colSpacing: (scrollBar.width/columns + wheelZoom) // Spacing between columns, aka quarter notes
    //This is just for horizontal lines that don't redraw ever
    onPaint: {
        var ctx = canvas2.getContext("2d")

        ctx.lineWidth = 1
        ctx.globalAlpha = 1
        if(!drewHorizLines){
            for(var i = 0; i < rows; i++){
                ctx.beginPath()
                ctx.strokeStyle = "light grey"
                ctx.moveTo(0,i*keyHeight)
                ctx.lineTo(canvas2.width,i*keyHeight)
                ctx.stroke()
            }
            drewHorizLines = true
        }

    }


    Canvas {
        id: canvas
        width: scrollBar.width
        height: pianoCol.height
        renderTarget: Canvas.FramebufferObject
        renderStrategy: Canvas.Threaded

        function addNotesFromFile(){

            var dw = 0
            var values = manager.getNoteInfo();
            for(var i = 1; i < values.length; i+=3){
                dw += values[i]

            }
            var tqn = dw/values[0]//tpqn is 0 pos for now, tqn tells total quarter notes
            totalCols = tqn//total qns
            testSpace = colSpacing
            tPQN = tqn
            var curNote = 0
            var elapsedDW = 0
            var noteEnd = 0
            var component = Qt.createComponent("PianoRollNote.qml")
            for(var i = 1; i < values.length; i+=3){


                elapsedDW += values[i]
                //indicates a note. ignore other junk for now
                if((values[i+2] & 0xF0) ===0x90){
                    curNote = (values[i+2] >> 8) & 127
                    //now I need to find its note off
                    for(var j = i+5; j< values.length; j+=3){
                        noteEnd+= values[j-2]
                        if(((values[j] >> 8) & 127) === curNote ){

                            component.createObject(canvas2,{"y":rows*keyHeight - keyHeight* curNote,"quad": elapsedDW/tqn,"id": totIds,"scale": scaleFactor,"noteStart": elapsedDW, "noteEnd": noteEnd});
                            totIds++
                            noteEnd = 0
                            break
                        }
                    }

                }
            }
            scrollBar.flickableItem.contentY = pianoCol.height/2
            canvas.requestPaint()


        }

        Component.onCompleted: {
            main.addNotes.connect(addNotesFromFile)
            testSpace = colSpacing
        }
        function drawGridLInes(ctx){
            if(colSpacing < 5){
            scaleFactor = 16

            }
            var lineEnd = scrollBar.flickableItem.contentY+scrollBar.height
            var lineBegin = scrollBar.flickableItem.contentY
            if(drewVerticals){
            lineEnd = canvas.height
                lineBegin = 0
            drewVerticals = false
            }
            var t = Math.floor((scrollBar.flickableItem.contentX /colSpacing) / scaleFactor)
            var tEnd = Math.floor(((scrollBar.flickableItem.contentX+scrollBar.width) /colSpacing) / scaleFactor)
            if(scaleFactor >= 1 && tEnd < scrollBar.width){
                tEnd++
            }

            ctx.beginPath();
            ctx.strokeStyle = "light grey"
            for(var i = t; i < tEnd; i++){
                if(i%(scaleFactor) === 0){
                }
                else{
                    continue
                }
                ctx.moveTo(i*colSpacing*scaleFactor,lineBegin);
                ctx.lineTo(i*colSpacing*scaleFactor,lineEnd);
            }

            ctx.stroke()
            ctx.beginPath();
            ctx.strokeStyle = "black"
            for(var i = t; i < tEnd; i++){
                if(i%(4/scaleFactor) === 0){
                    ctx.moveTo(i*colSpacing*scaleFactor,lineBegin);
                    ctx.lineTo(i*colSpacing*scaleFactor,lineEnd);
                }
            }
            ctx.stroke()
        }

        MouseArea{
            id: gridMouseArea
            hoverEnabled: true
            anchors.fill: parent
            property int yy: 0
            property int xx: 0
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onDoubleClicked: {
                if(mouse.button === Qt.LeftButton){
                    var component = Qt.createComponent("PianoRollNote.qml");
                    yy=  mouse.y / keyHeight
                    xx = mouse.x /(colSpacing * scaleFactor)

                    component.createObject(canvas2,{"note": yy,"quad": xx,"id": totIds,"scale": scaleFactor, "noteEnd": tPQN*scaleFactor});

                    updateMidi(yy,xx*tPQN,tPQN*scaleFactor)


                    totIds++
                }
            }
            onClicked: {
                activeId = -1
                if(mouse.button === Qt.RightButton){
                    menu.popup()
                }
            }

            onWheel: {

                var zoom = wheel.angleDelta.y / 120
                if(wheel.modifiers & Qt.ControlModifier){
                    drewVerticals = false
                    var lastX = colSpacing
                    //https://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio
                    var xoff = (((mouseX - scrollBar.flickableItem.contentX) * (2)) / (scrollBar.width)) - 1


                    wheelZoom += zoom
                    canvas.width = columns*colSpacing
                    var newX = Math.floor(mouseX / colSpacing)

                    scrollBar.flickableItem.contentX += newX * zoom
                      if(colSpacing < 3){
                      scrollBar.flickableItem.contentX = 0
                      }
                    if(colSpacing*columns < scrollBar.width )
                    {
                        wheelZoom = 0
                        canvas.width = scrollBar.width

                    }

                    testSpace = colSpacing
                    console.log( testSpace)
                    //canvas.requestPaint()
                }
                else{
                    scrollBar.flickableItem.contentY -= zoom * 20
                    if(!drewVerticals){
                     canvas.requestPaint()
                     drewVerticals = true
                    }

                }

            }

        }

        onPaint: {
            var ctx = canvas.getContext("2d")
            ctx.clearRect (0, 0, canvas.width, canvas.height);
            ctx.lineWidth = 1
            ctx.globalAlpha = 1
            drawGridLInes(ctx)

        }
        ScaleMenu{
            id:menu
            onAboutToHide: {
                var ctx = canvas.getContext("2d")
                ctx.clearRect (0, 0, canvas.width, canvas.height);
                canvas.requestPaint()

            }
        }

    }
}
