#include "midimanager.h"

MidiManager::MidiManager()
{

}
mSong song;
//QVector<int>noteVec = QVector<int>();
int MidiManager::TPQN = 960;

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
mSong MidiManager::Deserialize(QByteArray &array)
{
    //One extra byte at pos 0 of array for some reason, 0 based indexing cancels it out

    song.format = array.at(10);
    song.trackChunks = array.at(12); // no one needs more than 127 tracks anyways, technically this is var length
    qint16 bits;

    song.divisionFormat = (array.at(13) >> 7);
    if (song.divisionFormat)
    {
        song.deltaTimeSMTPE = array.at(13) & 127;
        song.framesPerSecondSMTPE = ~(array.at(14))+1;
        song.ticksPerQuarterNote = 0;
    }
    else
    {
        song.deltaTimeSMTPE = 0;
        song.framesPerSecondSMTPE = 0;
        bits = ((unsigned char)(array.at(13)) << 8) | (unsigned char)array.at(14);
        song.ticksPerQuarterNote = bits;
    }

    int currentPos = 15;
    if (true) { // was (if format is 0), but ya know who really cares
        //Runs for amount of tracks in a song
        for (int var = 0; var < song.trackChunks; ++var) {
            mTrack *track = new mTrack;
            track->trackName = "";
            track->instrumentName ="";
            currentPos += 4; //should put on first byte of Length
            track->length = ((unsigned char)(array.at(currentPos)) << 24 |
                             (unsigned char)(array.at(currentPos+1)) << 16 |
                             (unsigned char)(array.at(currentPos+2)) << 8 |
                             (unsigned char)(array.at(currentPos+3)));
            currentPos += 4; //should put on first byte after length

            mEvent lastEvent = mEvent();

            //Runs for amount of bytes in a track, each iteration is a new event
            for (int pos = currentPos; pos < track->length + currentPos; ++pos) {
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

                    pos+=2;
                }
                else if (array.at(pos+2) >> 7 == 0)
                {
                    deltaTime = ((unsigned char)(array.at(pos) ^ 128) << 14 |
                                 (unsigned char)(array.at(pos+1)) << 7 |
                                 (unsigned char)(array.at(pos+2)));

                    pos+=3;
                }
                else if ((unsigned char)array.at(pos+3) >> 7 == 0)
                {
                    deltaTime = ((unsigned char)(array.at(pos)^ 128) << 21 |
                                 (unsigned char)(array.at(pos+1)) << 14 |
                                 (unsigned char)(array.at(pos+2)) << 7 |
                                 (unsigned char)(array.at(pos+3)));
                    pos+=4;
                }
                mEvent event = mEvent();
                event.deltaTime = deltaTime;
                //Most of this is deprecated and should be removed
                //Running status
                if (((unsigned char)array.at(pos)  & 128) != 128) {
                    eventCanAdd = true;
                    event.dataByte1 = (unsigned char)array.at(pos);
                    event.channel = lastEvent.channel;
                    event.status = lastEvent.status;
                    event.dataByte2 = (unsigned char)array.at(++pos);
                    event.type = "note";
                    if (event.dataByte2 == 0) {
                        // event.noteOn = false;
                        event.status = 0x80 | event.channel;
                    }
                    // noteVec.append(statusDWORD(event.dataByte1,event.dataByte2,event.status));
                }
                int t = 0;
                t =  ((unsigned char)array.at(pos))>>4;
                if (!eventCanAdd) {//No need to run if running status
                    //Normal event
                    if (((unsigned char)array.at(pos) >>4) != 15) {
                        event.status = (unsigned char)array.at(pos);
                        event.channel =  (unsigned char)array.at(pos) & 15;
                        event.dataByte1 = (unsigned char)array.at(++pos);
                        event.dataByte2 = (unsigned char)array.at(++pos);
                        // noteVec.append(statusDWORD(event.dataByte1,event.dataByte2,event.status));
                        lastEvent = event;
                        eventCanAdd = true;
                    }
                    //Meta event
                    else if (((unsigned char)array.at(pos) >>4) == 15) {
                        switch ((unsigned char)array.at(++pos)) {
                        //Track name
                        case 3: {
                            int len = (unsigned char)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (unsigned char)array.at(++pos);
                            }
                            track->trackName = name;
                            break;
                        }
                            //Instrument name
                        case 4:{
                            int len = (unsigned char)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (unsigned char)array.at(++pos);
                            }
                            track->instrumentName = name;
                            break;
                        }

                            //Time signature
                        case 88:
                        {
                            int len = (unsigned char)array.at(++pos);
                            for (int var = 0; var < len; ++var) {

                                pos++;
                                //unhandled for now, wtf does the data mean

                            }
                            break;
                        }
                            //Key Signature
                        case 89:
                        {
                            int len = (unsigned char)array.at(++pos);
                            for (int var = 0; var < len; ++var) {

                                pos++;
                                //unhandled for now, wtf does the data mean
                            }
                            break;
                        }
                            //Tempo in microseconds per quarter note
                        case 81:
                        {
                            qDebug() << "tempo set unhandled";
                            int len = (unsigned char)array.at(++pos);
                            for (int var = 0; var < len; ++var) {

                                pos++;
                                //unhandled for now, wtf does the data mean
                            }
                            break;
                        }
                            //End of track
                        case 47:
                            pos =  track->length + currentPos;
                            break;
                        case 33:
                            pos++;
                            break;

                        default:
                            qDebug() << "Meta event unhandled: " << (unsigned char)array.at(pos);
                            break;
                        }
                    }
                }
                if (eventCanAdd) {
                    track->events.append(event);
                }
            }

            song.tracks.append(track);
        }
    }
    else if (song.format == 1) {

    }
    \

    //Note for later: Redesign everything to use [0] as first note, not the tpqn
    // This class can hold instances of all tracks i need, but don't rely
    //too heavily on its song structure.

    for (int curTrack = 0; curTrack < song.tracks.length(); ++curTrack) {
        \
        for (int var = 0; var < song.tracks.at(curTrack)->events.length(); ++var) {

            song.tracks[curTrack]->listOfNotes.append(song.tracks.at(curTrack)->events.at(var).deltaTime);
            song.tracks[curTrack]->listOfNotes.append(0);

            DWORD nvnt =( song.tracks.at(curTrack)->events.at(var).dataByte2 << 16 |
                          song.tracks.at(curTrack)->events.at(var).dataByte1 << 8 |
                          song.tracks.at(curTrack)->events.at(var).status);
            song.tracks[curTrack]->listOfNotes.append(nvnt);

        }
    }
    MidiManager::TPQN = song.ticksPerQuarterNote;
    emit notifyTrackViewChanged(&song);
    return song;
}
//There are a few cases where this fails, no idea why
void MidiManager::updateMidiAdd(int note,int veloc, int start, int length,mTrack *track){

    int vLen = track->listOfNotes.length();
    qDebug() << vLen;
    int vPos = 0;
    uchar status = 0x90;
    uchar velocity = veloc;
    int elapsedDT = 0;
    bool pastTheMark = false;
    bool beforeTheMark = false;
    bool hitTheMark = false;
    bool zeroDT = false;
    bool lastNote = false;
    QVector<int> newVec;
    if ( vLen > 0) {
        //runs twice - once for note start and another for note end
        for (int rep = 0; rep < 2; ++rep) {
            if (rep ==1) {
                //Housekeeping to get ready for next iteration
                start = start + length;
                velocity= 0;
                vLen = track->listOfNotes.length();
                pastTheMark = false;
                beforeTheMark = false;
                zeroDT = false;
                lastNote = false;
                newVec.clear();
                elapsedDT = 0;
            }

            //Start by getting the DT that elapsed before the Start value is hit
            //If I go over Start value, Determine whether Start is before or after current DT.
            //If I never go over, that means I placed value far right of all other notes.
            for (int i = 0; i < vLen; i+=3) {
                elapsedDT += track->listOfNotes.at(i);
                if (elapsedDT  >= start) {
                    if (elapsedDT == start) {
                        zeroDT = true;
                        vPos = i;
                        break;


                    }
                    elapsedDT -= track->listOfNotes.at(i);
                    if (elapsedDT < start) {
                        pastTheMark = true;
                        vPos = i-3;
                        elapsedDT = start - elapsedDT;

                    }
                    else{
                        beforeTheMark = true;
                        vPos = i-3;
                        elapsedDT = elapsedDT - start;
                    }
                    break;

                }
                else if(i+ 3 == vLen ){
                    lastNote = true;
                    vPos = i+3;
                    elapsedDT = start - elapsedDT;
                }

            }
            //Now reconstruct the original Vector one by one, putting in the new note start and end
            //where appropriate. Must determine whether new value comes before or after the current 'i' pos.
            // Also must adjust the DT of the next event if I put a new value BEFORE an existing one.

            for (int i = 0; i <= vLen; i+=3) {
                if (!lastNote && i == vLen) {
                    break; //gtfo

                }
                if (vPos == i) {
                    DWORD nvnt =( velocity << 16 |
                                  note << 8 |
                                  status);
                    //Only add the new note as vector will be maxed
                    if (lastNote) {
                        qDebug() << "LastNote Hit";
                        newVec.append(elapsedDT);
                        newVec.append(0);
                        newVec.append(nvnt);
                        break;
                    }
                    else if(zeroDT){
                         qDebug() << "zero DT - chord ";
                        newVec.append(track->listOfNotes.at(i));
                        newVec.append(track->listOfNotes.at(i+1));
                        newVec.append(track->listOfNotes.at(i+2));
                        newVec.append(0);
                        newVec.append(0);
                        newVec.append(nvnt);

                    }
                    //Add new note after existing event
                    else if (pastTheMark) {
                        qDebug() << "pastTheMark true Hit";
                        newVec.append(track->listOfNotes.at(i));
                        newVec.append(track->listOfNotes.at(i+1));
                        newVec.append(track->listOfNotes.at(i+2));
                        newVec.append(elapsedDT);
                        newVec.append(0);
                        newVec.append(nvnt);
                        hitTheMark = true;

                    }
                    //Add new event before existing, adjust the DT of existing
                    else if(beforeTheMark){
                        qDebug() << "pastTheMark false Hit";
                        newVec.append(elapsedDT);
                        newVec.append(0);
                        newVec.append(nvnt);
                        newVec.append(track->listOfNotes.at(i)-start);
                        newVec.append(track->listOfNotes.at(i+1));
                        newVec.append(track->listOfNotes.at(i+2));

                    }

                }
                // Add existing events as normal
                else{
                    //Seems this is needed for events that come inbetween longer notes
                    int PTMvar =0;
                    if (hitTheMark) {
                        PTMvar = elapsedDT;
                        hitTheMark =false;
                    }
                    newVec.append(track->listOfNotes.at(i)-PTMvar);
                    newVec.append(track->listOfNotes.at(i+1));
                    newVec.append(track->listOfNotes.at(i+2));

                }
            }
            if (rep == 0) {
                track->listOfNotes = newVec;
            }
        }

    }
    else
    {

        DWORD nvnt =( velocity << 16 |
                      note << 8 |
                      status);
        newVec.append(start);
        newVec.append(0);
        newVec.append(nvnt);
        nvnt =( 0 << 16 |
                note << 8 |
                0x90);
        newVec.append(length);
        newVec.append(0);
        newVec.append(nvnt);
    }
    track->listOfNotes = newVec;

}

DWORD MidiManager::statusDWORD(uchar db1, uchar db2, uchar status)
{
    DWORD nvnt =( db2 << 16 |
                  db1 << 8 |
                  status);
    return nvnt;

}

void MidiManager::updateMidiDelete(int start, int length, int note, mTrack *track)
{
    QVector<int> newVec;
    int vLen = track->listOfNotes.length();
    int DT = 0;
    int rep = 0;

    for (int pos = 0; pos < vLen; pos+=3)
    {
        uchar nte = track->listOfNotes.at(pos+2) >> 8;
        DT += track->listOfNotes.at(pos);
        if (DT == start && note == nte)
        {
            if (pos+3 <vLen)
            {
                track->listOfNotes[pos+3] += track->listOfNotes[pos];
            }
            if (rep == 0) {
              start = start + length;
            }
            rep++;
            continue;

        }
        else
        {
            newVec.append(track->listOfNotes.at(pos));
            newVec.append(track->listOfNotes.at(pos+1));
            newVec.append(track->listOfNotes.at(pos+2));
        }
    }

    track->listOfNotes = newVec;
}

//Great resource http://cs.fit.edu/~ryan/cse4051/projects/midi/midi.html#mff0
//http://www.mobilefish.com/tutorials/midi/midi_quickguide_specification.html
