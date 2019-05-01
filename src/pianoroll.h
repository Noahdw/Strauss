#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <src/midimanager.h>
#include <QHBoxLayout>
#include <src/trackwidget.h>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QTimer>
#include <src/trackmidiview.h>
#include "src/tracklengthview.h"
#include <QRubberBand>
#include <QKeyEvent>
#include <src/controlchangeoverlay.h>
#include <src/common.h>
#include "src/command.h"
#include <QStack>

class ControlChangeBridge;
class TrackMidi;
class PianoRollContainer;

class PianoRoll : public QGraphicsView{
    Q_OBJECT
public:
    PianoRoll(PianoRollContainer* p);
    ~PianoRoll();

    PianoRollContainer* container() {return _container;}
    void restoreTrack(TrackMidi* midiTrack);
    void playKeyboardNote(int note, bool active);
    void clearActiveNotes();
    void deleteAllNotes();
    TrackMidi* midiTrack() {return _midiTrack;}
    void resizeSelectedNotes(int xAdjustL,int xAdjustR);
    void deleteSelectedNotes();
    void setScrollWheelValue(int value);
    void updateSongTrackerPos(bool isPaused,bool isRestart, int custom);
    void notifyPianoRollItemMoved(int xMove, int yMove,QGraphicsItem *item);
    void addNoteToScene(int note, int position, int length, int velocity);
    void changeNotesAfterMouseDrag(QGraphicsItem *item);
    void forceResize();
    void issueMoveCommand(int xMove, int yMove, QGraphicsItem* item);
    void copyItems();
    void pasteItems();
    bool hasPlugin();
    TrackWidget *track;

    ControlChangeBridge * bridge;
    int tPQN = MidiManager::TPQN;
    int totalDT = MidiManager::TPQN*g_quarterNotes;
    double colSpacing = 0;
    double scaleFactor = 1;
    double prefferedScaleFactor = 1.0;
    bool isInitialized = false;
   // void updateWheel(QWheelEvent *event);
public slots:
    void ShowContextMenu(const QPoint &pos);
    void scaleFactorChanged(double scale);
    void convertTrackToItems();
    void turnNoteOff(int note);

protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    TrackMidi* _midiTrack;
    QGraphicsItemAnimation *animation;
    QRubberBand *rubberBand;
    QPoint origin;
    QList<QGraphicsItem*> lastSelectedItems;
    QList<QGraphicsItem*> copiedItems;
    QStack<Command*> commands;
    PianoRollContainer* _container;
    int currentTimer = 0;
    int lastYNote = 0;
    const double kminimumColSpacing = 20.0;
    double xscale = 1.1;
};





#endif // PIANOROLL_H
