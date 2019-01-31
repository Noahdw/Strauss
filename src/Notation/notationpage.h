#ifndef NOTATIONPAGE_H
#define NOTATIONPAGE_H

class NotationView;

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <src/Notation/measure.h>
class NotationPage : public QGraphicsView
{
    Q_OBJECT
public:
    NotationPage(NotationView *view);
    void addStave();
    int unusedMeasures();
    int staveGroupsPerPage();
    int measuresPerWidth();
    void assignMeasure(Measure *measure);
    std::map<Measure*,int> measures;
    int lineSpace = 30;
protected:
    void MousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent  *event);
   // void paintEvent(QPaintEvent * event);   // void mouseMoveEvent(QMouseEvent *event);
  //  void mouseReleaseEvent(QMouseEvent *event);
private:
    QGraphicsScene *scene;
    int prefferedMeasureWidth = 200;
    int pageWidth = 1200;
    int borderWidth = 100;
    int staveHeight = 150;
    int staveWidth;
    int numStaves = 0;
    int lastStavePos = staveHeight;
    NotationView *notationView;
};

#endif // NOTATIONPAGE_H
