#ifndef CONTROLCHANGEBRIDGE_H
#define CONTROLCHANGEBRIDGE_H

class ControlChangeView;
class ControlChangeOverlay;

#include <QWidget>
#include <QStackedLayout>
#include <QStackedWidget>
class ControlChangeBridge : public QWidget
{
    Q_OBJECT
public:
    ControlChangeBridge();
    QStackedLayout * sLayout;
    ControlChangeOverlay * overlay;
    ControlChangeView * view;
};

#endif // CONTROLCHANGEBRIDGE_H
