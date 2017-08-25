#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

#include <QObject>
#include <QWidget>
#include <midimanager.h>
#include <trackview.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <pianorollcontainer.h>
class TrackContainer : public QWidget
{
    Q_OBJECT
public:
    TrackContainer();
    void setPianoRollReference(PianoRollContainer *prc);
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
