#ifndef TRACKS_H
#define TRACKS_H
#include <QList>
#include "event.h"
class Tracks
{
public:
    Tracks();
    int length;
    QList<Event> events;
    QString trackName;
    QString instrumentName;
};

#endif // TRACKS_H
