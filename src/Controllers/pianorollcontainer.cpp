#include "pianorollcontainer.h"
#include "src/mainwindow.h"
#include "src/pianorollwidget.h"
#include "src/keyboard.h"
#include "src/velocityview.h"
#include "src/tracklengthview.h"
#include "src/trackmidi.h"

/*This class represents a collection of widgets whose uses are all linked to
 * one another. A Piano roll for editing MIDI, a Keyboard to for simple user
 * playback, and a Velocity view for editing velocities.
 *
 * */

PianoRollContainer::PianoRollContainer()
{
    mainLayout       = new QVBoxLayout;
    hLayout          = new QHBoxLayout;
    _pianoRoll       = new PianoRollWidget(this);
    _keyboard        = new Keyboard(this);
    _velocityView    = new VelocityView(this);
    _trackLengthView = new TrackLengthView(this);
    auto* hlayout2   = new QHBoxLayout;
    auto* hlayout3   = new QHBoxLayout;

    hLayout->addWidget(_keyboard);
    hLayout->addWidget(_pianoRoll);
    hlayout2->addSpacing(_keyboard->width());
    hlayout2->addWidget(_velocityView);
    hlayout2->addSpacing(qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent));
    hlayout3->addSpacing(_keyboard->width());
    hlayout3->addWidget(_trackLengthView);
    hlayout3->addSpacing(qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent));

    mainLayout->addLayout(hlayout3);
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(hlayout2);
    setLayout(mainLayout);

    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);

}

void PianoRollContainer::restoreTrack(TrackMidi* midiTrack)
{
    _currentMidiTrack = midiTrack;
    _pianoRoll->restoreTrack(midiTrack);
    _velocityView->restoreTrack(midiTrack);
}

void PianoRollContainer::playKeyboardNote(int note, bool active)
{
    auto velocity =  (active ? 45 : 0);
    _currentMidiTrack->addMidiEventToPlayback(0x90,note,velocity,g_timer->currentValue());
}

void PianoRollContainer::scaleWidgets(QMatrix matrix)
{

    _pianoRoll->setMatrix(matrix);
    _trackLengthView->setMatrix(matrix);
    _trackLengthView->setMatrix(matrix);
    auto m = _velocityView->matrix();
    m.setMatrix(matrix.m11(),m.m12(),m.m21(),m.m22(),0,0);
    _velocityView->setMatrix(m);

}

void PianoRollContainer::setScrollPositions(int value, Qt::Orientation axis)
{
    if(axis ==  Qt::Horizontal)
    {
        _pianoRoll->horizontalScrollBar()->setValue(value);
        _velocityView->horizontalScrollBar()->setValue(value);
        _trackLengthView->horizontalScrollBar()->setValue(value);
    }
    else
    {
        _pianoRoll->verticalScrollBar()->setValue(value);
        _keyboard->verticalScrollBar()->setValue(value);
    }
}

void PianoRollContainer::setScaleFactor(qreal scale)
{
    pianoRoll()->scaleFactor = scale;
    trackLengthView()->scaleFactor = scale;
    trackLengthView()->viewport()->update();
}
