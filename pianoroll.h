#ifndef PIANOROLL_H
#define PIANOROLL_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <midimanager.h>
#include <QHBoxLayout>
#include <trackview.h>
class PianoRoll : public QGraphicsView{
Q_OBJECT
public:
    PianoRoll(QWidget* parent = 0);
    void clearActiveNotes();
    void deleteAllNotes();
    TrackView *track;
    const QRectF *sceneRect;
    int cols = 50;
    double colSpacing = 0;
    int tPQN = 960;
    int totalDT = tPQN*cols;
    double scaleFactor = 1;


signals:
    void addNoteToPROLL(int note,int veloc,int start, int length, mTrack *track);
    void deleteNoteFromPROLL(int start, int length,int note, mTrack * track);
    void updateScrollWheel(int value);
    void updateVelocityViewItems(int start, int velocity,int note, bool adding);
    void setVelocityViewScale(float x,bool needsReset, int wheelPos);
public slots:
      void ShowContextMenu(const QPoint &pos);
      void scaleFactorChanged(double scale);
      void convertTrackToItems();
      void playKeyboardNote(int note, bool active);
protected:
      void mouseDoubleClickEvent(QMouseEvent  *event);
      void paintEvent(QPaintEvent * event);
      void mousePressEvent(QMouseEvent *event);
      void drawBackground(QPainter * painter, const QRectF & rect);
      void wheelEvent(QWheelEvent *event);
private:
      double xscale = 1.1;
      const double minimumColSpacing = 3;
      QGraphicsScene *scene;

};


#endif // PIANOROLL_H
