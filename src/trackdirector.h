#ifndef TRACKDIRECTOR_H
#define TRACKDIRECTOR_H

#include "QWidget"

class PianoRollContainer;
class ControlChangeContainer;
class MasterTrack;
class TrackMidi;
class FolderView;
class FolderViewAbstractModel;
class TrackContainer;
class TrackDirector
{
public:
    TrackDirector(MasterTrack* master, FolderViewAbstractModel *model);
    void setActiveTrack(TrackMidi* midiTrack);

    FolderView            * getFolderView(){return folderView;}
    TrackContainer        * getTrackContainer(){return trackContainer;}
    ControlChangeContainer* getControlChangeContainer() {return controlChangeContainer;}
    PianoRollContainer    * getPianoRollContainer() {return pianoRollContainer;}

private:
    PianoRollContainer *pianoRollContainer;
    ControlChangeContainer* controlChangeContainer;
    FolderView* folderView;
    MasterTrack* masterTrack;
    TrackContainer* trackContainer;
};

#endif // TRACKDIRECTOR_H
