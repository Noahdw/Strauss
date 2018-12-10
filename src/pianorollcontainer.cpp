#include "pianorollcontainer.h"
#include "src/mainwindow.h"
/*This class represents a collection of widgets whose uses are all linked to
 * one another. A Piano roll for editing MIDI, a Keyboard to for simple user
 * playback, and a Velocity view for editing velocities.
 *
 * */

PianoRollContainer::PianoRollContainer()
{      
    stackedLayout = new QStackedLayout;
    setLayout(stackedLayout);
}


// Can't load the same .dll from same folder so I copy to temp folder
// TODO: See if changing .dll name suffices and don't need to make x copies of a plugin, just 1


void PianoRollContainer::setControlChangeContainer(ControlChangeContainer *_controlChangeContainer)
{
    controlChangeContainer = _controlChangeContainer;
    for (int i = 0; i < stackedLayout->count(); ++i)
    {
        //  auto p =   dynamic_cast<PianoRoll*>(stackedLayout->->children().at(3))
    }
}

PianoRoll *PianoRollContainer::getCurrentPianoRoll()
{
    return dynamic_cast<PianoRoll*>(stackedLayout->currentWidget()->children().at(3));
}

void PianoRollContainer::switchPianoRoll(TrackView * track_view)
{
    if (getCurrentPianoRoll()->track == track_view)
    {
        //return;
    }
    for (int i = 0; i < stackedLayout->count(); ++i)
    {
        auto p = dynamic_cast<PianoRoll*>(stackedLayout->widget(i)->children().at(3));
        if (p->track == track_view)
        {
            stackedLayout->setCurrentIndex(i);
            controlChangeContainer->sLayout2->setCurrentIndex(i);
            break;
        }
    }



    for (int i = 0; i < stackedLayout->count(); i++)
    {
        QWidget *w = stackedLayout->itemAt(i)->widget();
        if (w)
        {
            auto proll = dynamic_cast<PianoRoll*>(w->children().at(3));
            proll->track->getTrackMidiView()->clickedOn(false);
        }

    }
    auto roll = getCurrentPianoRoll();
    roll->track->getTrackMidiView()->clickedOn(true);

}

PianoRoll *PianoRollContainer::addPianoRoll(TrackView *trackView)
{
    auto *pianoRoll     = new PianoRoll(trackView->midiTrack());
    auto *keyboard      = new Keyboard(pianoRoll);
    auto *velocityView  = new VelocityView(trackView);
    auto *trackLength   = new TrackLengthView;
    trackLength->pianoRoll = pianoRoll;
    pianoRoll->setKeyboard(keyboard);
    pianoRoll->setVelocityView(velocityView);
    pianoRoll->trackLengthView = trackLength;
    trackLength->initTrackLengthView(QRectF(0,0,trackView->midiTrack()->midiData()->totalDT,0),((float)trackLength->width() / (960*g_quarterNotes)));
    g_timer->setDuration(((float)(60.0/g_tempo)*g_quarterNotes*1000));
    qDebug() <<"Time: " << g_timer->duration();
    pianoRoll->track = trackView;
    if (trackView->midiTrack()->midiData()->totalDT != 0) {

        pianoRoll->convertTrackToItems();
    }

    auto *vlayout  = new QVBoxLayout;
    auto *hlayout  = new QHBoxLayout;
    auto *hlayout2 = new QHBoxLayout;
    auto *hlayout3 = new QHBoxLayout;
    hlayout3->addSpacing(keyboard->width());
    hlayout3->addWidget(trackLength);

    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout,0);

    hlayout->addWidget(keyboard);
    hlayout->addWidget(pianoRoll);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);

    hlayout2->addSpacing(keyboard->width());
    hlayout2->addWidget(velocityView);
    vlayout->addLayout(hlayout2);
    QFrame *initview = new QFrame;
    initview->setLayout(vlayout);
    stackedLayout->addWidget(initview);

    velocityView->setSceneRect(0,0,pianoRoll->totalDT,velocityView->height());
    velocityView->populateVelocityViewFromTrack(trackView);
    trackView->getTrackMidiView()->shareScene(pianoRoll->scene);
    controlChangeContainer->addControlChangeView(pianoRoll);
    pianoRoll->isInitialized = true;
    pianoRoll->forceResize();
    return pianoRoll;
}


