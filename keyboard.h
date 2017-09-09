#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <pianorollitem.h>
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QDebug>
class Keyboard: public QGraphicsView{
    Q_OBJECT
public:
    Keyboard(QWidget* parent = 0);
private:
    QGraphicsScene *scene;
    int noteWidth = 24;
    void addNotesToScene();
    QGraphicsRectItem *activeNote;
    QBrush activeBrush;
signals:
    void playSelectedNote(int note, bool active);
public slots:
    void scrollWheelChanged(int value);
protected:
      void mouseReleaseEvent(QMouseEvent *event);
      void mousePressEvent(QMouseEvent *event);
      void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // KEYBOARD_H
