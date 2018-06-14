#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

class TrackMidiView;

#include <QObject>
#include <QWidget>
#include <src/midimanager.h>
#include <src/trackview.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <src/pianorollcontainer.h>
#include <QSpacerItem>

class TrackContainer : public QWidget
{
    Q_OBJECT
public:
    TrackContainer();
    void setPianoRollReference(PianoRollContainer *prc);
    void addSingleView(TrackView *view);
public slots:
    void addTrackView(mSong *song);


signals:
    void addPianoRoll(TrackView *track);
    void requestTrackChange(int id);
private:
    QVBoxLayout *vLayout;
    QScrollArea *scrollArea;
protected:
   void mousePressEvent(QMouseEvent *event);
   PianoRollContainer *prcontainer;
};

#endif // TRACKCONTAINER_H
