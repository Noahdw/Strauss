#ifndef VELOCITYVIEW_H
#define VELOCITYVIEW_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>

class VelocityView : public QGraphicsView{
   Q_OBJECT

public:
    VelocityView(QWidget *parent =0);

public slots:
      void updateItems(int start, int velocity,int note, bool adding);
      void setScale(float x,bool needsReset,int wheelPos);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QGraphicsScene *scene;
};

#endif // VELOCITYVIEW_H
