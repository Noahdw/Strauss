#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <src/keyboard.h>
#include <src/pianoroll.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <src/midimanager.h>
#include <src/trackview.h>
#include <QStackedLayout>
#include <src/velocityview.h>
#include <src/tracklengthview.h>
#include <src/controlchangecontainer.h>
#include <QPainter>

class PianoRollContainer : public QWidget
{
    Q_OBJECT
public:
    PianoRollContainer();
    void propogateFolderViewDoubleClicked(QString pluginName, QString filePath, QString actualPath);
    void setControlChangeContainer(ControlChangeContainer *controlChangeContainer); // must be called before use
    void deletePianoRoll();
    PianoRoll *getCurrentPianoRoll();
    QStackedLayout *stackedLayout;
public slots:
    void switchPianoRoll(TrackView *track_view);
    void addPianoRolls(TrackView *trackView);
protected:
    void paintEvent(QPaintEvent * event);
private:
    QHBoxLayout *layout;
    ControlChangeContainer *control_change_container;

};

#endif // PIANOROLLCONTAINER_H
