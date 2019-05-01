#include "pianorollcontainer.h"
#include "src/mainwindow.h"
#include "pianoroll.h"
#include "keyboard.h"
#include "velocityview.h"
#include "tracklengthview.h"
#include "trackmidi.h"

/*This class represents a collection of widgets whose uses are all linked to
 * one another. A Piano roll for editing MIDI, a Keyboard to for simple user
 * playback, and a Velocity view for editing velocities.
 *
 * */

PianoRollContainer::PianoRollContainer()
{
    mainLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    _pianoRoll = new PianoRoll(this);
    _keyboard = new Keyboard(this);
    _velocityView = new VelocityView(this);
    _trackLengthView = new TrackLengthView(this);
    auto* hlayout2 = new QHBoxLayout;
    hLayout->addWidget(_keyboard);
    hLayout->addWidget(_pianoRoll);
    hlayout2->addSpacing(_keyboard->width());
    hlayout2->addWidget(_velocityView);
    mainLayout->addWidget(_trackLengthView);
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(hlayout2);
    setLayout(mainLayout);

    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);

}

void PianoRollContainer::restoreTrack(TrackMidi* midiTrack)
{
    _pianoRoll->restoreTrack(midiTrack);
    _velocityView->restoreTrack(midiTrack);
}


