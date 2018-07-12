#ifndef CONTROLCHANGECONTAINER_H
#define CONTROLCHANGECONTAINER_H

class PianoRollContainer;
class ControlChangeBridge;

#include <QObject>
#include <QWidget>
#include <src/controlchangeview.h>
#include <QVBoxLayout>
#include <QStackedLayout>

#include <QKeyEvent>

class ControlChangeContainer: public QWidget
{
    Q_OBJECT
public:
    ControlChangeContainer(PianoRollContainer * pRollCont);
    QStackedLayout *sLayout;
    QStackedLayout *sLayout2;
    QWidget * ccStackedHolder;
    void switchControlChangeContainer();
    void addControlChangeView(PianoRoll *pianoRoll);
    void switchControlChangeType(int index);
private:
    PianoRollContainer * pianoRollCont;

protected:
    void keyPressEvent(QKeyEvent * event);
};

#endif // CONTROLCHANGECONTAINER_H
