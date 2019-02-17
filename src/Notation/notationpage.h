#ifndef NOTATIONPAGE_H
#define NOTATIONPAGE_H

class NotationView;

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <src/Notation/stave.h>
#include <src/Notation/measure.h>
class NotationPage : public QGraphicsView
{
    Q_OBJECT
public:
    NotationPage(NotationView *view);
    Stave *addStave();
    int unusedMeasures();
    int staveGroupsPerPage();
    int measuresPerWidth();
    int staveYFromMeasure(Measure *measure);
    void assignMeasure(Measure *measure);
    void forceRedraw();
    std::map<Measure*,int> measures;
    QVector<Stave*> staves;
    int lineSpace = 30;
    NotationView *notationView;
protected:
    void MousePressEvent(QMouseEvent *event);

   // void paintEvent(QPaintEvent * event);   // void mouseMoveEvent(QMouseEvent *event);
  //  void mouseReleaseEvent(QMouseEvent *event);
private:
    Stave *getStaveFromScene(QPointF pos);
    Measure *getMeasureFromScene(Stave *stave, QPointF pos);
    QGraphicsScene *scene;
    int prefferedMeasureWidth = 200;
    int pageWidth = 1200;
    int borderWidth = 100;
    int staveHeight = 150;
    int staveWidth;
    int numStaves = 0;
    int lastStavePos = staveHeight;

};

#endif // NOTATIONPAGE_H
