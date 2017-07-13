#include "midiplayer.h"


MidiPlayer::MidiPlayer()
{

}
HANDLE hEvent;
HMIDISTRM outHandle;
void CALLBACK midiCallback(HMIDIOUT handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

void MidiPlayer::playMidiFile(Midi *song){
    int size = song->tracks.at(0).events.length();
    MIDIEVENT evnts[song->tracks.at(0).events.length()];
    MIDIPROPTIMEDIV prop;
    int tots = 0;
    MIDIEVENT event;
    for (int var = 0; var < size; ++var) {
        event.dwDeltaTime = song->tracks.at(0).events.at(var).deltaTime;
        event.dwStreamID = 0;
        DWORD nvnt =( (song->tracks.at(0).events.at(var).dataByte2 << 16 |
                       song->tracks.at(0).events.at(var).dataByte1 << 8 |
                       song->tracks.at(0).events.at(var).status));
        event.dwEvent = nvnt;

        evnts[tots] = event;
        tots++;
    }

    int tempNotes[tots*3];
    for (int var = 0; var < tots; ++var) {
        tempNotes[var*3] = evnts[var].dwDeltaTime;
        tempNotes[var*3 + 1] = 0;
        tempNotes[var*3 + 2] = evnts[var].dwEvent;
    }

    unsigned int DeviceID = 1;
    unsigned long result;

    hEvent = CreateEvent(0, FALSE, FALSE, 0);
    if (hEvent==NULL) {
        qDebug() << "CCould not creat event";
    }


    result = midiStreamOpen(&outHandle, &DeviceID, 1,(DWORD)midiCallback, 0, CALLBACK_FUNCTION );
    if (result!=MMSYSERR_NOERROR){
        qDebug() << "CCOULD NOT OPEN STREAM?";
    }
    prop.cbStruct = sizeof(MIDIPROPTIMEDIV);
    prop.dwTimeDiv = song ->ticksPerQuarterNote;
    midiStreamProperty(outHandle, (LPBYTE)&prop, MIDIPROP_SET|MIDIPROP_TIMEDIV);

    MIDIHDR buffer;
    MIDIHDR buffer1;
    MIDIHDR buffer2;


    const int tpqn = song->ticksPerQuarterNote;
    int totalTicks;
    bool songCanPlay = true;



    std::vector<int> v1,v2;
    for (int var = 0; var < sizeof(tempNotes)/sizeof(tempNotes[0]); var+=3) {
        totalTicks += tempNotes[var];
        if (totalTicks < tpqn) {
            v1.push_back(tempNotes[var]);
            v1.push_back(tempNotes[var + 1]);
            v1.push_back(tempNotes[var + 2]);
        }
        else {
            buffer1.lpData =(LPSTR)&v1[0];
            buffer1.dwBufferLength = sizeof(v1[0]) * v1.size();
            buffer1.dwBytesRecorded = sizeof(v1[0]) * v1.size();
            buffer1.dwFlags = 0;


            buffer = buffer1;
            result = midiOutPrepareHeader((HMIDIOUT)outHandle,&buffer,sizeof(buffer));
            if (result) {
                qDebug() << "midiOutPrepareHeader error:" << result;
            }
            for (int var = 0; var < 10; ++var) {

            }
            result = midiStreamOut(outHandle,&buffer,sizeof(buffer));
            if (result) {
                qDebug() << "midiStreamOut error:" << result;
            }
            result = midiStreamRestart(outHandle);
            if (result) {
                qDebug() << "midiStreamRestart error";

            }

            WaitForSingleObject(hEvent, INFINITE);
            v1.clear();
            v1.push_back(tempNotes[var]);
            v1.push_back(tempNotes[var + 1]);
            v1.push_back(tempNotes[var + 2]);
            totalTicks = 0;
        }




        midiOutUnprepareHeader((HMIDIOUT)outHandle, &buffer, sizeof(buffer));
    }
    buffer.lpData =(LPSTR)&v1[0];
    buffer.dwBufferLength = sizeof(v1[0]) * v1.size();
    buffer.dwBytesRecorded = sizeof(v1[0]) * v1.size();
    buffer.dwFlags = 0;





    midiStreamClose(outHandle);
    CloseHandle(hEvent);
}

void CALLBACK midiCallback(HMIDIOUT handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    LPMIDIHDR   lpMIDIHeader;
    MIDIEVENT * lpMIDIEvent;

    /* Determine why Windows called me */
    switch (uMsg)
    {
    /* Got some event with its MEVT_F_CALLBACK flag set */

    case MOM_POSITIONCB:

        /* Assign address of MIDIHDR to a LPMIDIHDR variable. Makes it easier to access the
               field that contains the pointer to our block of MIDI events */
        lpMIDIHeader = (LPMIDIHDR)dwParam1;

        /* Get address of the MIDI event that caused this call */
        lpMIDIEvent = (MIDIEVENT *)&(lpMIDIHeader->lpData[lpMIDIHeader->dwOffset]);

        /* Normally, if you had several different types of events with the
               MEVT_F_CALLBACK flag set, you'd likely now do a switch on the highest
               byte of the dwEvent field, assuming that you need to do different
               things for different types of events.
            */

        break;

        /* The last event in the MIDIHDR has played */
    case MOM_DONE:

        /* Wake up main() */
        SetEvent(hEvent);

        break;


        /* Process these messages if you desire */
    case MOM_OPEN:
    case MOM_CLOSE:

        break;
    }
}

void MidiPlayer::pausePlayBack(){
    midiStreamPause(outHandle);
}

void MidiPlayer::resumePlayBack(){
    midiStreamRestart(outHandle);;
}

