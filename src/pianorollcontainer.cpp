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
void PianoRollContainer::propogateFolderViewDoubleClicked(QString pluginName, QString filePath)
{
  PianoRoll *roll = dynamic_cast<PianoRoll*>(stackedLayout->currentWidget()->children().at(3));
  roll->track->folderViewItemDoubleClicked(filePath,pluginName);
}

PianoRoll *PianoRollContainer::getPianoRollRef()
{
    return dynamic_cast<PianoRoll*>(stackedLayout->currentWidget()->children().at(3));
}

void PianoRollContainer::switchPianoRoll(int id)
{
    if (stackedLayout->currentIndex() != id) {
        stackedLayout->setCurrentIndex(id);
        ccContainer->sLayout2->setCurrentIndex(id);
        for (int i = 0; i < stackedLayout->count(); i++)
        {
             QWidget *w = stackedLayout->itemAt(i)->widget();
             if (w)
             {
                 auto proll = dynamic_cast<PianoRoll*>(w->children().at(3));
                 proll->track->trackMidiView->clickedOn(false);
             }

        }
        auto roll = getPianoRollRef();
                 roll->track->trackMidiView->clickedOn(true);
    }
}

void PianoRollContainer::addPianoRolls(TrackView *view)
{
    auto *roll        = new PianoRoll;
    auto *key         = new Keyboard;
    auto *velocity    = new VelocityView;
    auto *trackLength = new TrackLengthView;

    roll->track = view;
    if (view->track->totalDT != 0) {

        roll->convertTrackToItems();
    }

    auto *vlayout  = new QVBoxLayout;
    auto *hlayout  = new QHBoxLayout;
    auto *hlayout2 = new QHBoxLayout;
    auto *hlayout3 = new QHBoxLayout;
    hlayout3->addSpacing(key->width());
    hlayout3->addWidget(trackLength);

    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout,0);

    hlayout->addWidget(key);
    hlayout->addWidget(roll);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);


    hlayout2->addSpacing(key->width());
    hlayout2->addWidget(velocity);
    vlayout->addLayout(hlayout2);

    QWidget *initview = new QWidget;
    initview->setLayout(vlayout);
    stackedLayout->addWidget(initview);

    roll->setKeyboard(key);
    roll->setVelocityView(velocity);
    roll->trackLengthView = trackLength;
    trackLength->initTrackLengthView(QRectF(0,0,view->track->totalDT,0),((float)trackLength->width() / (960*g_quarterNotes)));

    key->setPianoRoll(roll);
    view->plugin.host->setPianoRollRef(roll);
    velocity->setSceneRect(0,0,roll->totalDT,velocity->height());
    velocity->populateVelocityViewFromTrack(view);
    velocity->trackView = view;
    view->trackMidiView->shareScene(roll->scene);
    ccContainer->addControlChangeView(roll);
}

void PianoRollContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(Qt::darkGray);
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height());
}
