#ifndef PIANOROLLWIDGET_H
#define PIANOROLLWIDGET_H

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

class PianoRollWidget : public QGraphicsView {
    Q_OBJECT
public:
    PianoRollWidget(PianoRollContainer* p);
    ~PianoRollWidget();

    PianoRollContainer* container() {return _container;}
    void restoreTrack(TrackMidi* midiTrack);

    void clearActiveNotes();
    void deleteAllNotes();
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
    TrackMidi* midiTrack() {return _midiTrack;}
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
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawBackground(QPainter * painter, const QRectF & rect) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

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





#endif // PIANOROLLWIDGET_H
