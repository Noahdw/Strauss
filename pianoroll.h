#ifndef PIANOROLL_H
#define PIANOROLL_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>

class PianoRoll : public QGraphicsView{
Q_OBJECT
public:
    PianoRoll(QWidget* parent = 0);

    const QRectF *sceneRect;
signals:
    void addNoteToPROLL(int x,int y,int width,int start, int length);
    void deleteNotesFromPROLL(QGraphicsItem * item);

public slots:
      void ShowContextMenu(const QPoint &pos);
      void scaleFactorChanged(double scale);
protected:
      void mouseDoubleClickEvent(QMouseEvent  *event);
      void paintEvent(QPaintEvent * event);
      void mousePressEvent(QMouseEvent *event);
      void drawBackground(QPainter * painter, const QRectF & rect);
      void wheelEvent(QWheelEvent *event);

};


#endif // PIANOROLL_H
