// Button.qml
import QtQuick 2.4

Column {


    Rectangle {
        id:rect1
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        Component.onCompleted: {

            objectName = globalKeys;
            globalKeys-=12;

        }
        border { width: 1; color: "black" }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect1.objectName,"on")

                pressed= 0;
            }
            onReleased: {
                playNote(rect1.objectName,"off")
                pressed = 1;
            }

            onEntered: {
                parent.color = "red"
                if(pressed === 0)
                    playNote(rect1.objectName,"on")
            }

            onExited: {
                parent.color = "white"
            }
        }
    }
    Rectangle {
        id: rect2
        width: keyWidth;
        height: keyHeight;
        color: "black"
        objectName: globalKeys;
        Component.onCompleted: {

            objectName = globalKeys - 1;
        }
        border { width: 1; color: "black" }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect2.objectName,"on")
                console.log(rect2.objectName);
                pressed = 0
            }
            onReleased: {
                playNote(rect2.objectName,"off")
                pressed = 1;
            }
            onEntered: {
                parent.color = "red"
                if(pressed === 0)
                    playNote(rect2.objectName,"on")
            }

            onExited: {
                parent.color = "black"
            }
        }
    }

    Rectangle {
        id: rect3
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 2;
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect3.objectName,"on")
                console.log(rect3.objectName);
                pressed = 0;
            }
            onReleased: {
                playNote(rect3.objectName,"off")
                pressed = 1;
            }
            onEntered: {
                parent.color = "red"
                if(pressed === 0)
                    playNote(rect3.objectName,"on")
            }

            onExited: {
                parent.color = "white"
            }
        }
    }
    Rectangle {
        id: rect4
        width: keyWidth;
        height: keyHeight;
        color: "black"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 3;
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect4.objectName,"on")
                console.log(rect4.objectName);
            }
            onReleased: {
                playNote(rect4.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "black"
            }

        }
    }
    Rectangle {
        id: rect5
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys -4;
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect5.objectName,"on")
                console.log(rect5.objectName);
            }
            onReleased: {
                playNote(rect5.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "white"
            }
        }
    }
    Rectangle {
        id: rect6
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 5;
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect6.objectName,"on")
                console.log(rect6.objectName);
            }
            onReleased: {
                playNote(rect6.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "white"
            }
        }
    }
    Rectangle {
        id:rect7
        width: keyWidth;
        height: keyHeight;
        color: "black"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 6;
        }


        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect7.objectName,"on")
                console.log(rect7.objectName);
            }
            onReleased: {
                playNote(rect7.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "black"
            }
        }
    }

    Rectangle {
        id:rect8
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 7;
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect8.objectName,"on")
                console.log(rect8.objectName);
            }
            onReleased: {
                playNote(rect8.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "white"
            }
        }
    }

    Rectangle {
        id:rect9
        width: keyWidth;
        height: keyHeight;
        color: "black"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 8;
        }


        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect9.objectName,"on")
                console.log(rect9.objectName);
            }
            onReleased: {
                playNote(rect9.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "black"
            }
        }
    }
    Rectangle {
        id:rect10
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 9;
        }
        border { width: 1; color: "black" }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect10.objectName,"on")
                console.log(rect10.objectName);
            }
            onReleased: {
                playNote(rect10.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "white"
            }
        }
    }

    Rectangle {
        id:rect11
        width: keyWidth;
        height: keyHeight;
        color: "black"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {
            objectName = globalKeys - 10;
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect11.objectName,"on")
                console.log(rect11.objectName);
            }
            onReleased: {
                playNote(rect11.objectName,"off")
            }
            onEntered: {
                parent.color = "red"
            }

            onExited: {
                parent.color = "black"
            }
        }
    }
    Rectangle {
        id:rect12
        width: keyWidth;
        height: keyHeight;
        color: "white"
        objectName: globalKeys;
        border { width: 1; color: "black" }
        Component.onCompleted: {

            objectName = globalKeys - 11;
        }
        border { width: 1; color: "black" }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                playNote(rect12.objectName,"on")
                console.log(rect12.objectName);
            }
            onReleased: {
                playNote(rect12.objectName,"off")
            }
            onEntered: {
                parent.color = "red"

            }

            onExited: {
                parent.color = "white"
            }
        }
    }



}


