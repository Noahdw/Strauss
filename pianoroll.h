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
    void convertFileToItems(MidiManager &manager );
    void deleteAllNotes();
    TrackView *track;
    const QRectF *sceneRect;
    int cols = 50;
    double colSpacing = 0;
    int tPQN = 120;
    int totalDT = tPQN*cols;
    double scaleFactor = 1;

signals:
    void addNoteToPROLL(int x,int y,int width,int start, int length);
    void updateScrollWheel(int value);

public slots:
      void ShowContextMenu(const QPoint &pos);
      void scaleFactorChanged(double scale);
      void convertTrackToItems();
protected:
      void mouseDoubleClickEvent(QMouseEvent  *event);
      void paintEvent(QPaintEvent * event);
      void mousePressEvent(QMouseEvent *event);
      void drawBackground(QPainter * painter, const QRectF & rect);
      void wheelEvent(QWheelEvent *event);
private:
      double xscale = 1.1;
      double minimumColSpacing = 3;
      QGraphicsScene *scene;

};


#endif // PIANOROLL_H
