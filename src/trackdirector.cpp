#include "trackdirector.h"
#include "Controllers/controlchangecontainer.h"
#include "Controllers/pianorollcontainer.h"
#include "mastertrack.h"
#include "folderview.h"
#include "trackcontainer.h"
#include "mastertrack.h"

TrackDirector::TrackDirector(MasterTrack *master, FolderViewAbstractModel *model) : masterTrack(master)
{
    pianoRollContainer = new PianoRollContainer;
    controlChangeContainer = new ControlChangeContainer(master,pianoRollContainer);
    folderView = new FolderView(model,master);
    trackContainer = new TrackContainer(master,this);
}

void TrackDirector::setActiveTrack(TrackMidi* midiTrack)
{
    masterTrack->setCurrentTrack(midiTrack);
    pianoRollContainer->restoreTrack(midiTrack);
}
