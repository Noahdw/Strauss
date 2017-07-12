#include "midimanager.h"

MidiManager::MidiManager()
{

}

QByteArray MidiManager::ReadMidi(QFile &file)
{
    QDataStream in(&file);

    QByteArray array;
    qint8 test;
    in>> test;
    QString  s = QString::number(test);
    array.append(s);

    for (int var = 0; var < file.size(); ++var)
    {
        in >> test;
        array.append(test);
    }
    return array;

}
Midi MidiManager::Deserialize(QByteArray &array)
{
    //One extra byte at pos 0 of array for some reason, 0 based indexing cancels it out
    Midi song;
    song.format = array.at(10);
    song.trackChunks = array.at(12); // no one needs more than 127 tracks anyways
    qint16 bits;

    song.divisionFormat = (array.at(13) >> 7);
    if (song.divisionFormat)
    {
        song.deltaTimeSMTPE = array.at(13) & 127;
        song.framesPerSecondSMTPE = ~(array.at(14))+1;
    }
    else
    {
        bits = ((unsigned char)(array.at(13)) << 8) | (unsigned char)array.at(14);
        song.ticksPerQuarterNote = bits;
    }

    int currentPos = 15;
    if (true) { // was if format is 0, but ya know
        //Runs for amount of tracks in a song
        for (int var = 0; var < song.trackChunks; ++var) {
            Tracks track;
            currentPos += 4; //should put on first byte of Length
            track.length = ((unsigned char)(array.at(currentPos)) << 24 |
                            (unsigned char)(array.at(currentPos+1)) << 16 |
                            (unsigned char)(array.at(currentPos+2)) << 8 |
                            (unsigned char)(array.at(currentPos+3)));
            currentPos += 4; //should put on first byte after length

            Event lastEvent;

            //Runs for amount of bytes in a track, each iteration is a new event
            for (int pos = currentPos; pos < track.length + currentPos; ++pos) {
                bool  eventCanAdd = false;
                //A delta tima(variable length 1 - 4 bytes) always comes before an event
                //Because variable length, bit 7(last MSB) is used to denote that it is last byte
                //in the series if it is 0.
                //NOTE* bit 7n must be 1 if it is not last byte, does not signify a larger value though
                int deltaTime;
                if (array.at(pos) >> 7 == 0)
                {
                    deltaTime =array.at(pos);
                    pos++;
                }
                else if (array.at(pos+1) >> 7 == 0)
                {
                    //A bit messy but inverts the bit 7n values, shifts the last bit 1 right (makes second to last a 0)
                    deltaTime = (unsigned char)(array.at(pos) ^ 128) << 7;
                    deltaTime = (deltaTime | ((unsigned char)array.at(pos+1)));
\
                    pos+=2;
                }
                else if (array.at(pos+2) >> 7 == 0)
                {
                    deltaTime = ((unsigned char)(array.at(pos)) << 16 |
                                 (unsigned char)(array.at(pos+1)) << 8 |
                                 (unsigned char)(array.at(pos+2))) << 2;
                    deltaTime = (deltaTime >> 2) | 32896;
                    pos+=3;
                }
                else if ((unsigned char)array.at(pos+3) >> 7 == 0)
                {
                    deltaTime = ((unsigned char)(array.at(pos)) << 24 |
                                 (unsigned char)(array.at(pos+1)) << 16 |
                                 (unsigned char)(array.at(pos+2)) << 8 |
                                 (unsigned char)(array.at(pos+3))) << 2;
                    deltaTime = (deltaTime >> 2) | 8421504;
                    pos+=4;
                }
                Event event;
                event.deltaTime = deltaTime;
                //Running status
                if (((unsigned char)array.at(pos)  & 128) != 128) {
                    eventCanAdd = true;
                    event.dataByte1 = (unsigned char)array.at(pos);
                    event.channel = lastEvent.channel;
                    event.status = lastEvent.status;
                    event.dataByte2 = (unsigned char)array.at(++pos);
                    event.type = "note";
                    if (event.dataByte2 == 0) {
                        event.noteOn = false;
                        event.status = 0x80 | event.channel;
                    }
                }
                int t = 0;
                t =  ((unsigned char)array.at(pos))>>4;
                if (!eventCanAdd) {//No need to run if running status
                    switch (t) {
                    //Note on
                    case 9:
                        event.status = (unsigned char)array.at(pos);
                        event.channel =  (unsigned char)array.at(pos) & 15;
                        event.noteOn = true;
                        event.dataByte1 = (unsigned char)array.at(++pos);
                        event.dataByte2 = (unsigned char)array.at(++pos);
                        event.type = "note";
                        lastEvent = event;
                        eventCanAdd = true;
                        break;
                        //Note off
                    case 8:
                        qDebug() << "Note off handeled";
                        event.status = (unsigned char)array.at(pos);
                        event.channel =  (unsigned char)array.at(pos) & 15;
                        event.noteOn = true;
                        event.dataByte1 = (unsigned char)array.at(++pos);
                        if ((unsigned char)array.at(++pos) != 0) {
                            qDebug() << "Note off was not 0 velocity";
                        }
                        event.dataByte2 = 0;
                        event.type = "note";
                        lastEvent = event;
                        eventCanAdd = true;
                        break;
                        //General purpose DAW stuff
                    case 11:
                        pos++;
                        switch ((unsigned char)array.at(pos)) {
                        //Sustain pedal
                        case 64:
                            eventCanAdd = true;
                            event.type = "note";
                            event.status = (unsigned char)array.at(pos-1);
                            event.channel =  (unsigned char)array.at(pos-1) & 15;
                            event.dataByte1 = (unsigned char)array.at(pos);
                            event.dataByte2 = (unsigned char)array.at(++pos);
                            lastEvent.status = event.status;
                            break;
                        default:
                            qDebug() << "Unhandled DAW event:" << (unsigned char)array.at(pos);
                            break;
                        }
                        break;
                        // Meta event
                    case 15:
                        pos++;
                        switch ((unsigned char)array.at(pos)) {
                        //Track name
                        case 3: {
                            int len = (unsigned char)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (unsigned char)array.at(++pos);
                            }
                            track.trackName = name;
                            break;
                        }
                        case 4:{
                            int len = (unsigned char)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (unsigned char)array.at(++pos);
                            }
                            track.instrumentName = name;
                            break;

                        }
                        //End of track
                        case 47:
                          pos =  track.length + currentPos;
                            break;
                        default:
                            qDebug() << "Unhandled meta event:" << (unsigned char)array.at(pos);
                            break;

                        }
                        break;
                    default:
                        qDebug() << "Unhandled event:" << t;
                        break;
                    }


                }
                if (eventCanAdd) {
                    track.events.append(event);
                }
            }

            song.tracks.append(track);

        }
    }
    else if (song.format == 1) {

    }
    return song;
}

//Great resource http://cs.fit.edu/~ryan/cse4051/projects/midi/midi.html#mff0
//http://www.mobilefish.com/tutorials/midi/midi_quickguide_specification.html
