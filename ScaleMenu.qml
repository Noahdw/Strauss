import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Menu {
    title: "Edit"
    GridLayout{
        columns:  5

        MenuItem {
            text: "2 bar"
            onTriggered:{
              scaleFactor = 8
            }
        }
        MenuItem {
            text: "1 bar"
            onTriggered:{
              scaleFactor = 4
            }

        }

        MenuItem {
            text: "1/2 bar"
          onTriggered:{
            scaleFactor = 2
          }
        }

        MenuItem {
            text: "1/4 bar"
            onTriggered:{
              scaleFactor = 1
            }
        }
        MenuItem {
            text: "1/8 bar"
            onTriggered:{
              scaleFactor = .5
            }
        }
        MenuItem {
            text: "1/16 bar"
            onTriggered:{
              scaleFactor = .25
            }
        }
        MenuItem {
            text: "1/32 bar"
            onTriggered:{
              scaleFactor = .125
            }
        }
    }


    MenuSeparator { }

    Menu {
        title: "More Stuff"

        MenuItem {
            text: "Do Nothing"
        }
    }
}
