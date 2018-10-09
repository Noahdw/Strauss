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
    //  setMinimumWidth(1000);
    this->setLayout(stackedLayout);
}


// Can't load the same .dll from same folder so I copy to temp folder
// TODO: See if changing .dll name suffices and don't need to make x copies of a plugin, just 1
void PianoRollContainer::propogateFolderViewDoubleClicked(QString pluginName, QString filePath, QString actualPath)
{
    if (stackedLayout->count() == 0)
    {
        return;
    }
    PianoRoll *roll = dynamic_cast<PianoRoll*>(stackedLayout->currentWidget()->children().at(3));
    roll->track->addPluginFromPath(filePath,pluginName,actualPath);
}

void PianoRollContainer::setControlChangeContainer(ControlChangeContainer *controlChangeContainer)
{
    control_change_container = controlChangeContainer;
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
            control_change_container->sLayout2->setCurrentIndex(i);
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

void PianoRollContainer::addPianoRolls(TrackView *trackView)
{
    auto *pianoRoll     = new PianoRoll;
    auto *keyboard      = new Keyboard(pianoRoll);
    auto *velocityView  = new VelocityView(trackView);
    auto *trackLength   = new TrackLengthView;

    g_timer->setDuration(((float)(60.0/g_tempo)*g_quarterNotes*1000));
    qDebug() <<"Time: " << g_timer->duration();
    pianoRoll->track = trackView;
    if (trackView->track->totalDT != 0) {

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

    QWidget *initview = new QWidget;
    initview->setLayout(vlayout);
    stackedLayout->addWidget(initview);

    pianoRoll->setKeyboard(keyboard);
    pianoRoll->setVelocityView(velocityView);
    pianoRoll->trackLengthView = trackLength;
    trackLength->initTrackLengthView(QRectF(0,0,trackView->track->totalDT,0),((float)trackLength->width() / (960*g_quarterNotes)));

    trackView->plugin.host->setPianoRollRef(pianoRoll);
    velocityView->setSceneRect(0,0,pianoRoll->totalDT,velocityView->height());
    velocityView->populateVelocityViewFromTrack(trackView);
    trackView->getTrackMidiView()->shareScene(pianoRoll->scene);
    control_change_container->addControlChangeView(pianoRoll);
    pianoRoll->isInitialized = true;
    pianoRoll->forceResize();
}

void PianoRollContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(Qt::darkGray);
    painter.setBrush(brush);
    painter.drawRect(0,0,width() - 1,height() - 1);
}
