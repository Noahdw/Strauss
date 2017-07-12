#ifndef EVENT_H
#define EVENT_H
#include <QFileDialog>
#include <QFile>

class Event
{
public:
    Event();

    int deltaTime;
   unsigned char dataByte1;
   unsigned char dataByte2;
   unsigned char channel;
   unsigned char status;
    bool noteOn;
    QString type;
};

#endif // EVENT_H
