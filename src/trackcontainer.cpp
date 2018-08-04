#include "trackcontainer.h"
#include <QWidget>
#include <src/trackmidiview.h>
#include "src/plugineditorcontainer.h"
int ID = 0;
/*This class represents a container for the tracks, located above the Pianoroll.
 *
 *
 **/
TrackContainer::TrackContainer(PluginEditorContainer *pluginEditorContainer, PianoRollContainer *pianoRollContainer)
{
    plugin_editor_container = pluginEditorContainer;
    piano_roll_container = pianoRollContainer;
    vSplitter = new QSplitter(this);
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->setContentsMargins(0,0,0,0);
    vSplitter->setHandleWidth(0);
    vSplitter->setStyleSheet("QSplitter {background-color: rgb(170,170,170); border: 1px black; }");
    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addLayout(vLayout);
    auto lt = new QVBoxLayout;
    lt->setSpacing(0);
    lt->setContentsMargins(0,0,0,0);
    lt->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    lt->addWidget(vSplitter);
    hLayout->addLayout(lt);
    setLayout(hLayout);

    QObject::connect(this,&TrackContainer::addPianoRoll,pianoRollContainer,&PianoRollContainer::addPianoRolls);

}

void TrackContainer::addTrackView(const mSong &song)
{
    foreach (const auto &track, song.tracks)
    {
        QWidget *widget = new QWidget;
        auto *midiView = new TrackMidiView;
        auto *view = new TrackView(track,midiView);
        auto *hlayout = new QHBoxLayout;
        hlayout->setAlignment(Qt::AlignTop);
        hlayout->setSpacing(0);
        hlayout->setContentsMargins(0,0,0,0);
        widget->setLayout(hlayout);
        hlayout->addWidget(view);

        view->id = ID;
        ID++;

        emit addPianoRoll(view);
        QObject::connect(view,&TrackView::trackClickedOn,piano_roll_container,&PianoRollContainer::switchPianoRoll);
        vLayout->addWidget(midiView);
        vSplitter->addWidget(view);
        view->pluginTrack = plugin_editor_container->addTrack(view);
    }
    adjustSize();
}


void TrackContainer::addSingleView()
{
    mTrack *track = new mTrack;
    auto *midiView = new TrackMidiView;
    track->instrumentName = "New Track";
    TrackView *view = new TrackView(track,midiView);
    view->id = ID++;

    emit addPianoRoll(view);
    QObject::connect(view,&TrackView::trackClickedOn,piano_roll_container,&PianoRollContainer::switchPianoRoll);

    vLayout->addWidget(midiView);
    vSplitter->addWidget(view);
    view->pluginTrack = plugin_editor_container->addTrack(view);

}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

void TrackContainer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_C:
        switchControlChange();
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void TrackContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(Qt::darkGray);
    painter.setBrush(brush);
    painter.setPen(QPen(Qt::darkGray));
    painter.drawRect(0,0,width() - 1,height() - 1);
}

