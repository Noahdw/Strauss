#ifndef PIANOROLL_H
#define PIANOROLL_H

//Forward declarations
class Keyboard;
class VelocityView;

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
#include <QRubberBand>
class PianoRoll : public QGraphicsView{
Q_OBJECT
public:
    PianoRoll(QWidget* parent = 0);
    TrackView *track;
    Keyboard *getKeyboard();
    VelocityView *velocityView;
     QRectF *sceneRect;
    double colSpacing = 0;
    double scaleFactor = 1;
    int cols = 60;
    int tPQN = 960;
    int totalDT = tPQN*cols;

    void setKeyboard(Keyboard *kboard);
    void setVelocityView(VelocityView *view);
    void playKeyboardNote(int note, bool active);
    void clearActiveNotes();
    void deleteAllNotes();
    void setScrollWheelValue(int value);
    void updateSongTrackerPos(bool isPauseOrResume, bool isResume);
    void notifyPianoRollItemMoved(int xMove, int yMove,QGraphicsItem *item);

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
private:
      double xscale = 1.1;
      int currentTimer = 0;
      const double minimumColSpacing = 3;
      QGraphicsScene *scene;
      Keyboard *keyboard;
      QTimeLine *timer;
      QGraphicsItemAnimation *animation;
      int lastYNote = 0;
      QRubberBand *rubberBand;
      QPoint origin;

};


#endif // PIANOROLL_H
