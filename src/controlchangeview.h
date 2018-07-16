#ifndef CONTROLCHANGEVIEW_H
#define CONTROLCHANGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QPainter>
#include <QStackedLayout>
#include <QMouseEvent>
#include <QDebug>
#include <src/pianoroll.h>

class ControlChangeView  : public QGraphicsView{
    Q_OBJECT

public:
    ControlChangeView(QWidget* parent = 0);
     QGraphicsScene * scene = nullptr;
     PianoRoll * pRoll;
     std::map<int,int> ccValueMap;

protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void showEvent(QShowEvent *event);
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);
private:

};

#endif // CONTROLCHANGEVIEW_H
