import QtQuick 2.0
import QtQml 2.2


Canvas {
    id: canvas
    width: scrollBar.width;
    height: pianoCol.height;
    property int rows: 132
    property int columns: totalCols
    property int totIds: 0
    property int wheelZoom: 0
    property int colSpacing: (pianoCol.height/columns + wheelZoom) // Spacing between columns, aka quarter notes
    renderTarget: Canvas.FramebufferObject
    renderStrategy: Canvas.Threaded
    function addNotesFromFile(){
        var dw = 0
        var values = manager.getNoteInfo();
        for(var i = 1; i < values.length; i+=3){
            dw += values[i]

        }
        var tqn = dw/values[0]//tpqn is 0 pos for now, tqn tells total quarter notes
        totalCols = tqn
        var curNote = 0
        var elapsedDW = 0
        var noteEnd = 0
        for(var i = 1; i < values.length; i+=3){
            //indicates a note. ignore other junk for now
            elapsedDW += values[i]

            if((values[i+2] & 0xF0) ===0x90){
                curNote = (values[i+2] >> 8) & 127
               //now I need to find its note off
                for(var j = i+5; j< values.length; j+=3){
                    noteEnd+= values[j-2]
                    if(((values[j] >> 8) & 127) === curNote ){
                         var component = Qt.createComponent("PianoRollNote.qml");
                         component.createObject(canvas,{"y": keyHeight* curNote,"quad": elapsedDW/tqn,"id": totIds,"scale": scaleFactor,"noteStart": elapsedDW, "noteEnd": noteEnd+elapsedDW});
                        totIds++
                        noteEnd = 0
                        console.log("found")
                        break
                    }
                }

            }
        }



    }

  Component.onCompleted: {
    main.addNotes.connect(addNotesFromFile)
  }
    function drawGridLInes(ctx){
        for(var i = 0; i < rows; i++){
            ctx.beginPath();
            ctx.strokeStyle = "light grey"
            ctx.moveTo(0,i*keyHeight);
            ctx.lineTo(canvas.width,i*keyHeight);
            ctx.stroke()
        }
        for(var i = 0; i < columns / scaleFactor; i++){

            if(i%(4/scaleFactor) === 0){
                ctx.strokeStyle = "black"
            }
            else if(i%(scaleFactor) === 0){
                ctx.strokeStyle = "light grey"
            }
            else{
                continue
            }
            ctx.beginPath();
            ctx.moveTo(i*colSpacing*scaleFactor,0);
            ctx.lineTo(i*colSpacing*scaleFactor,canvas.height);
            ctx.stroke()
        }
    }

    MouseArea{
        id: gridMouseArea
        anchors.fill: parent
        property int yy: 0
        property int xx: 0
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onDoubleClicked: {
            if(mouse.button === Qt.LeftButton){
                var component = Qt.createComponent("PianoRollNote.qml");
                yy=  mouse.y / keyHeight
                xx = mouse.x /(colSpacing * scaleFactor)

                component.createObject(canvas,{"y": keyHeight* yy,"quad": xx,"id": totIds,"scale": scaleFactor});

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
            if(wheel.modifiers & Qt.ControlModifier){
                var ctx = canvas.getContext("2d")
                ctx.clearRect (0, 0, canvas.width, canvas.height);
                wheelZoom +=  wheel.angleDelta.y / 120
                canvas.width = columns*colSpacing
                if(colSpacing*columns < scrollBar.width )
                {

                    wheelZoom -=  wheel.angleDelta.y / 120
                    canvas.width = columns*colSpacing
                }

                testSpace = colSpacing
                canvas.requestPaint()
            }
            else{


            }
        }

    }

    onPaint: {
        var ctx = canvas.getContext("2d")

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
