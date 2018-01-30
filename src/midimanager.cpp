#include "midimanager.h"

MidiManager::MidiManager()
{
}
mSong song;
//QVector<int>noteVec = QVector<int>();
//960 seems to be a standard tpqn for recording midi
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
    int ticksPerQuarterNote,framesPerSecondSMTPE,deltaTimeSMTPE;
    int format = array.at(10);
    int trackChunks = array.at(12); // no one needs more than 127 tracks anyways, technically this is var length

    bool divistionFormat = (array.at(13) >> 7);
    if (divistionFormat)
    {
        deltaTimeSMTPE = array.at(13) & 127;
        framesPerSecondSMTPE = ~(array.at(14))+1;
        ticksPerQuarterNote = 0;
    }
    else
    {
        deltaTimeSMTPE = 0;
        framesPerSecondSMTPE = 0;
        ticksPerQuarterNote = ((uchar)(array.at(13)) << 8) | (uchar)array.at(14);
    }
    double tpqnScale = TPQN / ticksPerQuarterNote; // Enforce 960 TPQN
    int currentPos = 15;
    // was (if format is 0), only TPQN supported for now though
    if (true) {
        //Runs for amount of tracks in a song
        for (int var = 0; var < trackChunks; ++var) {
            mTrack *track = new mTrack;
            track->trackName = "";
            track->instrumentName ="";
            currentPos += 4; //should put on first byte of Length
            track->length = ((uchar)(array.at(currentPos)) << 24 |
                             (uchar)(array.at(currentPos+1)) << 16 |
                             (uchar)(array.at(currentPos+2)) << 8 |
                             (uchar)(array.at(currentPos+3)));
            currentPos += 4; //should put on first byte after length

            uchar lastStatus,lastChannel;
            //Runs for amount of bytes in a track, each iteration is a new event
            for (int pos = currentPos; pos < track->length + currentPos; ++pos) {
                bool  eventCanAdd = false;

                //A delta tima(variable length 1 - 4 bytes) always comes before an event
                //Because of variable length, bit 7(last MSB) is used to denote that it is last byte
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
                    deltaTime = (uchar)(array.at(pos) ^ 128) << 7;
                    deltaTime = ((uchar)deltaTime | ((uchar)array.at(pos+1)));

                    pos+=2;
                }
                else if (array.at(pos+2) >> 7 == 0)
                {
                    deltaTime = ((uchar)(array.at(pos) ^ 128) << 14 |
                                 (uchar)(array.at(pos+1)) << 7 |
                                 (uchar)(array.at(pos+2)));

                    pos+=3;
                }
                else if ((uchar)array.at(pos+3) >> 7 == 0)
                {
                    deltaTime = ((uchar)(array.at(pos)^ 128) << 21 |
                                 (uchar)(array.at(pos+1)) << 14 |
                                 (uchar)(array.at(pos+2)) << 7 |
                                 (uchar)(array.at(pos+3)));
                    pos+=4;
                }

                deltaTime*=tpqnScale;
                track->totalDT += deltaTime;

                //Running status
                if (((uchar)array.at(pos)  & 128) != 128) {
                    eventCanAdd = true;
                    uchar dataByte1 = (uchar)array.at(pos);
                    uchar channel = lastChannel;
                    uchar status = lastStatus;
                    uchar dataByte2 = (uchar)array.at(++pos);
                    if (dataByte2 == 0) {
                        // event.noteOn = false;
                        status = 0x80 | channel;
                    }
                    DWORD nvnt =( dataByte2 << 16 |
                                  dataByte1 << 8 |
                                  status);
                    track->listOfNotes.append(deltaTime);
                    track->listOfNotes.append(0);
                    track->listOfNotes.append(nvnt);
                }

                //No need to run if running status
                if (!eventCanAdd) {
                    //Normal event
                    if (((uchar)array.at(pos) >>4) != 15) {
                        uchar status = (uchar)array.at(pos);
                        uchar channel =  (uchar)array.at(pos) & 15;
                        uchar dataByte1 = (uchar)array.at(++pos);
                        uchar dataByte2 = (uchar)array.at(++pos);
                        DWORD nvnt =( dataByte2 << 16 |
                                      dataByte1 << 8 |
                                      status);
                        eventCanAdd = true;

                        track->listOfNotes.append(deltaTime);
                        track->listOfNotes.append(0);
                        track->listOfNotes.append(nvnt);
                        lastChannel = channel;
                        lastStatus = status;
                    }

                    //Meta event
                    else if (((uchar)array.at(pos) >> 4) == 15) {

                        switch ((uchar)array.at(++pos)) {
                        //Track name
                        case 3: {
                            int len = (uchar)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (uchar)array.at(++pos);
                            }
                            track->trackName = name;
                            break;
                        }
                            //Instrument name
                        case 4:{
                            int len = (uchar)array.at(++pos);
                            QString name;
                            name.resize(len);
                            for (int var = 0; var < len; ++var) {
                                name[var] = (uchar)array.at(++pos);
                            }
                            track->instrumentName = name;
                            break;
                        }

                            //Time signature
                        case 88:
                        {
                            int len = (uchar)array.at(++pos);
                            for (int var = 0; var < len; ++var) {

                                pos++;
                                //unhandled for now, wtf does the data mean

                            }
                            break;
                        }
                            //Key Signature
                        case 89:
                        {
                            int len = (uchar)array.at(++pos);
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
                            int len = (uchar)array.at(++pos);
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
                            qDebug() << "Meta event unhandled: " << (uchar)array.at(pos);
                            break;
                        }
                    }
                }

            }
            qDebug() << "TPQN: " << track->totalDT << " LENGTH: " << track->listOfNotes.length();
            song.tracks.append(track);
        }
    }
    else if (song.format == 1) {
        //Not supported
    }

    emit notifyTrackViewChanged(&song);
    return song;
}
//There are a few cases where this fails, no idea why
//Can be made way simpler
void MidiManager::updateMidiAdd(int note,int veloc, int start, int length,mTrack *track){

    int vLen = track->listOfNotes.length();
    qDebug() << vLen;
    int vPos  = 0;
    int newDT = 0;
    int runs = 0;
    uchar status = 0x90;
    uchar velocity = veloc;
    int elapsedDT = 0;
    bool lastNote = false;
    QVector<int> newVec;
    if ( vLen > 0) {
        //runs twice - once for note start and another for note end
        for (int rep = 0; rep < 2; ++rep) {
            if (rep ==1) {
                //Housekeeping to get ready for next iteration
                start = start + length;
                velocity= 0;
                lastNote = false;
            }

            //Start by getting the DT that elapsed before the Start value is hit
            //If I go over Start value, Determine whether Start is before or after current DT.
            //If I never go over, that means I placed value far right of all other notes.
            for (int i = vPos; i <= vLen; i+=3) {
                if(i == vLen){
                    lastNote = true;
                    newDT = std::abs(elapsedDT - start);
                    break;
                }
                elapsedDT += track->listOfNotes.at(i);
                if (elapsedDT  >= start) {
                    vPos = i;
                    newDT = std::abs(elapsedDT - start);
                    break;
                }
                else if(i+ 3 == vLen ){
                    lastNote = true;
                    vPos = i+3;
                    newDT = std::abs(elapsedDT - start);
                    break;
                }
            }
            //Now reconstruct the original Vector one by one, putting in the new note start and end
            //where appropriate. Must determine whether new value comes before or after the current 'i' pos.
            // Also must adjust the DT of the next event if I put a new value BEFORE an existing one.

            if(runs > vLen){
                runs = vLen;
            }
            for (int i = runs; i <= vLen; i+=3) {
                //   qDebug() << "VPOS " << vPos;
                if (!lastNote && i == vLen) {
                    break; //gtfo
                }

                if (vPos == i) {
                    runs = i+3;
                    DWORD nvnt =( velocity << 16 |
                                      note << 8  |
                                          status);
                    if (lastNote) {
                        qDebug() << "LastNote Hit";
                        newVec.append(newDT);
                        newVec.append(0);
                        newVec.append(nvnt);
                        if(rep == 0){
                            break;
                        }
                    }
                    else{
                        if(newDT == 0){
                            // Notice that a note on needs to be after, and vise versa.
                            if(rep == 0){
                                newVec.append(track->listOfNotes.at(i));
                                newVec.append(track->listOfNotes.at(i+1));
                                newVec.append(track->listOfNotes.at(i+2));
                                newVec.append(0);
                                newVec.append(0);
                                newVec.append(nvnt);
                            }
                            else{
                                newVec.append(track->listOfNotes.at(i));
                                newVec.append(0);
                                newVec.append(nvnt);
                                newVec.append(0);
                                newVec.append(track->listOfNotes.at(i+1));
                                newVec.append(track->listOfNotes.at(i+2));
                            }
                        }
                        else{
                            newVec.append(track->listOfNotes.at(i)-newDT);
                            newVec.append(0);
                            newVec.append(nvnt);
                            newVec.append(newDT);
                            newVec.append(track->listOfNotes.at(i+1));
                            newVec.append(track->listOfNotes.at(i+2));
                        }
                        if(rep == 0){
                            vPos +=3;
                            break;
                        }
                    }
                }
                // Add existing events as normal
                else{
                    newVec.append(track->listOfNotes.at(i));
                    newVec.append(track->listOfNotes.at(i+1));
                    newVec.append(track->listOfNotes.at(i+2));
                }
            }

        }
    }
    else
    {
        DWORD nvnt =( velocity << 16 |
                          note << 8  |
                              status);
        newVec.append(start);
        newVec.append(0);
        newVec.append(nvnt);
        nvnt =(0 << 16 |
            note << 8  |
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
                  db1 << 8  |
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
