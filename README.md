# MidiInter



MidiInter is the beginning of what will hopefully one day be a Digital Audio Workstation that focuses
on composer workflow through the integration of both notation software as well as traditional DAWS.

![midiinter_2018-01-29_17-05-06](https://user-images.githubusercontent.com/1783601/35542779-be31dcc4-0516-11e8-944b-044557acd43c.png)



### Some insights for those looking to create a piano roll with Qt.
The naive approach to creating the structure behind most piano rolls will not work. That is to say, the manual 
updating of every note width and xPos when "zooming" in won't work. This approach involves constantly resizing 
the screenRect when the user zooms (more of an illusion really), iterationg through every item in the scene, and
mapping its elapsed ticks (note start) and the ticks since the start (note length) to the current scene. E.g. A note
with 400 elapsed delta time (DT) and a length of 120 DT being maped from  the range(0,totalTicksInSong) to (0,sceneWidth). 
If you try this approach you will find it unbearably slow and often lossy in the graphical representation of the item.


The way to make it work is less straightforward but still surprisingly simple. Rather than constantly mapping
an item to the scene, you instead create a scene with a width that is the total ticks in your song. Add an item
easily by setting its xPos and width in the scene with just the DT info you already have. Now instead of when
zooming in with the wheel and changing the sceneRect, you call QGraphicViews scale(x,y). So you can do something 
like scale(zoomFactor,1) to zoom in and scale(1/Zoomfactor,1) to zoom out. It's important to note that scale()
simply multiplies the views matrix scaleFactor by the supplied value. To determine how zoomed in you actually are
you can call transform().m11(). Some work is needed to get the view into the correct scale at as startup or to limit
how much you can zoom out. this->scale(((float)width() / (tPQN*cols)),1); works well for me - where tPQN * cols is 
really just the total ticks of your song.

A few final notes to get zooming to behave correctly. setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
works well to zoom in at your current mouse location, like most DAWS do, just make sure to detemine your current
verticalScrollbar value before zooming, and then setting it again afterwards. You will zoom in the Y direction if 
you don't do so. Also don't attempt to make something like this with QML as I did. It won't work.



