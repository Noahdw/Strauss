#include "midimanager.h"
#include "src/common.h"
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
    int highest_total_dt = TPQN * 60;
    double tpqnScale = TPQN / ticksPerQuarterNote; // Enforce 960 TPQN
    int currentPos = 15;
    // was (if format is 0), only TPQN supported for now though, SMTPE support latter
    if (true) {
        for (int var = 0; var < trackChunks; ++var) {
            MidiData *track = new MidiData;
            currentPos += 4; //should put on first byte of Length
            track->length = ((uchar)(array.at(currentPos))   << 24 |
                             (uchar)(array.at(currentPos+1)) << 16 |
                             (uchar)(array.at(currentPos+2)) << 8  |
                             (uchar)(array.at(currentPos+3)));
            qDebug() << "Track " << var << "length: " << track->length;
            currentPos += 4; //should put on first byte after length

            uchar lastStatus = 0,lastChannel = 0;
            int pos;
            //Runs for amount of bytes in a track, each iteration is a new event
            for (pos = currentPos; pos < track->length + currentPos; ++pos) {
                bool  eventCanAdd = false;

                //A delta tima(variable length 1 - 4 bytes) always comes before an event
                //Because of variable length, bit 7(last MSB) is used to denote that it is last byte
                //in the series if it is 0.
                //NOTE* bit 7n must be 1 if it is not last byte, does not signify a larger value though
                //TODO: Convert to a loop
                int deltaTime = 0;
                if ((uchar)array.at(pos) >> 7 == 0)
                {
                    deltaTime = (uchar)array.at(pos);
                    pos++;
                }
                else if ((uchar)array.at(pos+1) >> 7 == 0)
                {
                    deltaTime = (uchar)(array.at(pos) ^ 0x80) << 7;
                    deltaTime = (deltaTime | ((uchar)array.at(pos+1)));
                    if (deltaTime > 960)
                    {
                        qDebug() <<"DT: " <<  deltaTime;
                    }
                    pos+=2;
                }
                else if ((uchar)array.at(pos+2) >> 7 == 0)
                {
                    deltaTime = ((uchar)(array.at(pos) ^ 0x80) << 14 |
                                 (uchar)(array.at(pos+1)) << 7 |
                                 (uchar)(array.at(pos+2)));

                    pos+=3;
                }
                else if ((uchar)array.at(pos+3) >> 7 == 0)
                {
                    deltaTime = ((uchar)(array.at(pos)^ 0x80) << 21 |
                                 (uchar)(array.at(pos+1)) << 14 |
                                 (uchar)(array.at(pos+2)) << 7 |
                                 (uchar)(array.at(pos+3)));
                    pos+=4;
                }

                deltaTime *= tpqnScale;
                track->totalDT += deltaTime;

                //Running status
                if (((uchar)array.at(pos)  & 0x80) != 0x80) {
                    eventCanAdd = true;
                    if (lastStatus == 0xB0) //IGNORE CC FOR NOW
                    {
                        pos +=1;
                        continue;
                    }
                    else if(lastStatus == 0xC0) // IGNORE PC FOR NOW
                    {
                        continue;
                    }
                    else if (lastStatus == 0xA0)
                    {
                        pos += 1;
                        continue;
                    }
                    else if (lastStatus == 0xD0)
                    {
                        continue;
                    }
                    uchar dataByte1 = (uchar)array.at(pos);
                    uchar dataByte2 = (uchar)array.at(++pos);
                    if (dataByte2 == 0) {
                        // event.noteOn = false;
                      //  status = 0x80 | channel;
                    }
                    DWORD nvnt =( dataByte2 << 16 |
                                  dataByte1 << 8 |
                                  lastStatus);
                    addPartialMidiNote(nvnt,track->totalDT,track);
                }

                //No need to run if running status
                if (!eventCanAdd) {
                    //Normal event
                    if (((uchar)array.at(pos) >>4) != 0xF) {
                        uchar status = (uchar)array.at(pos) & 0xF0;
                        lastStatus = status;
                        if (status == 0xB0) //IGNORE CC FOR NOW
                        {
                            pos +=2;
                            continue;
                        }
                        else if(status == 0xC0) // IGNORE PC FOR NOW
                        {
                            pos +=1;
                            continue;
                        }
                        else if (status == 0xA0)
                        {
                            pos += 2;
                            continue;
                        }
                        else if (status == 0xD0)
                        {
                            pos += 1;
                            continue;
                        }
                        else if (status != 0x90 && status != 0x80)
                        {
                           qDebug() << "Status not implemented: " << status;
                        }
                        uchar channel =  (uchar)array.at(pos) & 0xF;
                        uchar dataByte1 = (uchar)array.at(++pos);
                        uchar dataByte2 = (uchar)array.at(++pos);

                        DWORD nvnt =( dataByte2 << 16 |
                                      dataByte1 << 8 |
                                      status);
                        eventCanAdd = true;

                        addPartialMidiNote(nvnt,track->totalDT,track);
                        lastChannel = channel;
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
                                //unhandled for now

                            }
                            break;
                        }
                            //Key Signature
                        case 89:
                        {
                            int len = (uchar)array.at(++pos);
                            for (int var = 0; var < len; ++var) {

                                pos++;
                                //unhandled for now
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
                                //unhandled for now
                            }
                            break;
                        }
                            //End of track
                        case 47:
                            pos =  track->length + currentPos;
                            break;
                        case 33:
                            pos+=2;
                            break;

                        default:
                            qDebug() << "Meta event unhandled: " << (uchar)array.at(pos);
                            break;
                        }
                    }
                }

            }
            currentPos = pos - 1;
            recalculateNoteListDT(track);
            qDebug() << "TOTALDT: " << track->totalDT << " LENGTH: " << track->listOfNotes.length();
            if (track->totalDT > highest_total_dt)
            {
                highest_total_dt = track->totalDT;
            }
            song.tracks.append(track);
        }
    }
    else if (song.format == 1) {
        //Not supported
    }
    for (int i = 0; i < song.tracks.size(); ++i)
    {
        song.tracks[i]->totalDT = highest_total_dt;
    }
    g_quarterNotes = (double)highest_total_dt / TPQN;
   // emit notifyTrackViewChanged(&song);
    return song;
}
//This simply adds a note to the map, it does not update the actual midi data itself
//call MidiManager::recalculateNoteListDT after this in order to create the necessary
//midi structure. The map contains totalDeltaTime -> midiEvent pairs and the listOfNotes,
//which is the actual structure used for playback, is groups of deltaTime, midiEvents.
void MidiManager::addMidiNote(int note,int velocity, int start, int length,MidiData *track)
{
    DWORD event =( velocity << 16 |
                   note << 8  |
                   0x90);
    Q_ASSERT(track);
    track->noteMap[start].push_back(event);
    track->noteMap[start+length].push_back(event & 0x00FFFF);
}

void MidiManager::addPartialMidiNote(DWORD event, int start, MidiData *track)
{
    track->noteMap[start].push_back(event);
}

void MidiManager::removeMidiNote(int start, int length, int note, MidiData *track)
{
    for (uint var = 0; var < track->noteMap.at(start).size(); ++var)
    {
        if (((track->noteMap.at(start).at(var) >> 8) & 0xFF) == note)
        {
            if (track->noteMap.at(start).size() == 1)
            {
                track->noteMap.erase(start);
            }
            else
            {
                track->noteMap.at(start).erase(track->noteMap.at(start).begin()+var);

            }
            break;
        }
    }
    for (uint var = 0; var < track->noteMap.at(start+length).size(); ++var)
    {
        if (((track->noteMap.at(start+length).at(var) >> 8) & 0xFF) == note)
        {
            if (track->noteMap.at(start+length).size() == 1)
            {
                track->noteMap.erase(start+length);
            }
            else
            {
                track->noteMap.at(start+length).erase(track->noteMap.at(start+length).begin() + var);
            }
            break;
        }
    }
}

void MidiManager::changeMidiVelocity(int start, int note, int velocity, MidiData *track)
{
    for (uint var = 0; var < track->noteMap.at(start).size(); ++var)
    {
        if (((track->noteMap.at(start).at(var) >> 8) & 0xFF) == note)
        {
            track->noteMap[start].at(var) &= 0xFF00FFFF;
            track->noteMap[start].at(var) |= velocity << 16;
            MidiManager::recalculateNoteListDT(track);
            return;
        }
    }

}



void MidiManager::recalculateNoteListDT(MidiData *track)
{
    track->listOfNotes.clear();
    track->listOfNotes.reserve(track->noteMap.size()*2);
    int counter = 0;
    int last = 0;
    for(const auto& var : track->noteMap)
    {
        for (uint i = 0; i < var.second.size(); ++i)
        {
            if (counter == 0)
            {
                track->listOfNotes.push_back(var.first);
                track->listOfNotes.push_back(var.second.at(i));
                last = var.first;
                counter++;
            }
            else
            {
                track->listOfNotes.push_back(var.first - last);
                track->listOfNotes.push_back(var.second.at(i));
                last = var.first;
            }
        }
    }
}

int MidiManager::getVelocityFromNote(int start, int note, MidiData *track)
{
    for (uint var = 0; var < track->noteMap.at(start).size(); ++var)
    {
        qDebug() << "ORIG NOTE: " << note << " OTHER NOTE: " << ((track->noteMap.at(start).at(var) >> 8) & 0xFF);
        if (((track->noteMap.at(start).at(var) >> 8) & 0xFF) == note)
        {
            return track->noteMap.at(start).at(var) >> 16;
        }
    }
    return -1;
}

//Great resource http://cs.fit.edu/~ryan/cse4051/projects/midi/midi.html#mff0
//http://www.mobilefish.com/tutorials/midi/midi_quickguide_specification.html
