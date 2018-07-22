#ifndef CONTROLCHANGEBRIDGE_H
#define CONTROLCHANGEBRIDGE_H

class ControlChangeView;
class ControlChangeOverlay;

#include <QWidget>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QGraphicsScene>

class ControlChangeBridge : public QWidget
{
    Q_OBJECT
public:
    ControlChangeBridge();
    void setCurrentOverlay(int index);
    void verifyOverlayExists(int value);
    QStackedLayout * sLayout;
    std::vector<ControlChangeOverlay*> overlays;
    //ControlChangeOverlay * overlay;
    ControlChangeView * view;
    int currentIndex = 0;
    QGraphicsScene *baseScene;
private:

};

#endif // CONTROLCHANGEBRIDGE_H
