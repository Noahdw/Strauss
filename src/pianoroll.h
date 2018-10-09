#ifndef PIANOROLL_H
#define PIANOROLL_H

//Forward declarations
class Keyboard;
class VelocityView;
class ControlChangeBridge;

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <src/midimanager.h>
#include <QHBoxLayout>
#include <src/trackview.h>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QTimer>
#include <src/trackmidiview.h>
#include "src/tracklengthview.h"
#include <QRubberBand>
#include <QKeyEvent>
#include <src/controlchangeoverlay.h>
#include <src/common.h>
#include <src/timetracker.h>

class PianoRoll : public QGraphicsView{
    Q_OBJECT
public:
    PianoRoll(QWidget* parent = 0);
    ~PianoRoll();
    Keyboard* getKeyboard();
    void setKeyboard(Keyboard *kboard);
    void setVelocityView(VelocityView *view);
    void playKeyboardNote(int note, bool active);
    void clearActiveNotes();
    void deleteAllNotes();
    void deleteSelectedNotes();
    void setScrollWheelValue(int value);
    void updateSongTrackerPos(bool isPauseOrResume, bool isResume, int custom);
    void notifyPianoRollItemMoved(int xMove, int yMove,QGraphicsItem *item);
    void addNoteToScene(int note, int position, int length, int velocity);
    void changeNotesAfterMouseDrag(QGraphicsItem *item);
    void switchViewContainer();
    void forceResize();
    TrackView *track;
    TrackLengthView * trackLengthView;
    VelocityView *velocityView;
    QGraphicsScene *scene;
    QRectF *sceneRect;

    ControlChangeBridge * bridge;
    int tPQN = MidiManager::TPQN;
    int totalDT = MidiManager::TPQN*g_quarterNotes;
    double colSpacing = 0;
    double scaleFactor = 1;
    double prefferedScaleFactor = 1.0;
    bool isInitialized = false;

public slots:
    void ShowContextMenu(const QPoint &pos);
    void scaleFactorChanged(double scale);
    void convertTrackToItems();
    void turnNoteOff(int note);

protected:
    void mouseDoubleClickEvent(QMouseEvent  *event);
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    Keyboard *keyboard;
    QGraphicsItemAnimation *animation;
    QRubberBand *rubberBand;
    QGraphicsRectItem *line;
    QPoint origin;
    QList<QGraphicsItem*> last_selected_items;
    int currentTimer = 0;
    int lastYNote = 0;
    const double kminimumColSpacing = 20.0;
    double xscale = 1.1;
};


#endif // PIANOROLL_H
