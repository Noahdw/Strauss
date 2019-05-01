#ifndef TRACKMIDI_H
#define TRACKMIDI_H

#include "common.h"
#include "src/midimanager.h"
#include "QGraphicsScene"
#include <QGraphicsRectItem>
#include <QList>
class PianoRoll;
class TrackWidget;
class MasterTrack;
class AudioPlugin;
class Vst2AudioPlugin;
class ControlChange;

class MidiEditorState
{
public:
    MidiEditorState()
    {
        pianoRollScene = new QGraphicsScene(0,0,g_totalDt,128* 12 );
        pianoRollScene->setItemIndexMethod(QGraphicsScene::NoIndex);
        line       = new QGraphicsRectItem(0,0,1,5000);
        line->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
        pianoRollScene->addItem(line);
        line->setZValue(1);
        QPen pen(QColor(156,21,25),0);
        line->setPen(pen);
        line->setBrush(QColor(156,21,25));
        line->setPos(0,-50);

        velocityViewScene = new QGraphicsScene(0,0,g_totalDt,128);
    }

    QGraphicsScene* pianoRollScene;
    QGraphicsScene* velocityViewScene;
       QGraphicsRectItem* line;
private:

};

class TrackMidi
{
public:
    TrackMidi(MasterTrack* mTrack);

    ~TrackMidi();
    void setTrackView(TrackWidget *trackView);
    void prepareToDelete();
    void addPlugin(QString actualPath);

    void setCanRecord(bool state) {_canRecord = state;}
    bool canRecord() {return _canRecord;}
    void setMuted(bool state) {_muted = state;}
    bool isMuted() {return _muted;}
    MidiEditorState* midiEditorState(){return &_midiEditorState;}
    TrackWidget*  trackView(){return _trackView;}
    MidiData*   midiData(){return &_midiData;}
    PianoRoll*  pianoRoll(){return _pianoRoll;}\
    MidiData*   ccAt(int index);
    bool addCC(int index);
    std::map<int,MidiData> &cc() {return _cc;}
    AudioPlugin* plugin(){return _plugin.get();}
    AudioPlugin* masterPlugin(){return _plugin.get();}
    QList<Vst2AudioPlugin*> effectPlugins; // TODO
    ControlChange* _controlChange;
private:
    MidiEditorState _midiEditorState;
    TrackWidget * _trackView;
    MidiData  _midiData;
    std::map<int,MidiData> _cc;
    PianoRoll* _pianoRoll;
    std::unique_ptr<AudioPlugin> _plugin;
    MasterTrack *masterTrack;
    bool _canRecord = false;
    bool _muted = false;

};


#endif // TRACKMIDI_H
