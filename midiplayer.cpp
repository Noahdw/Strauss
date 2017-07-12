#include "midiplayer.h"


MidiPlayer::MidiPlayer()
{

}
HANDLE hEvent;
void CALLBACK midiCallback(HMIDIOUT handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
void MidiPlayer::playMidiFile(Midi *song){
    int size = song->tracks.at(0).events.length();
    MIDIEVENT evnts[song->tracks.at(0).events.length()];
    MIDIPROPTIMEDIV prop;
    int tots = 0;
    MIDIEVENT event;
    for (int var = 0; var < size; ++var) {

        if (song->tracks.at(0).events.at(var).type == "note") {

            event.dwDeltaTime = song->tracks.at(0).events.at(var).deltaTime;
            event.dwStreamID = 0;
            DWORD nvnt =( (song->tracks.at(0).events.at(var).dataByte2 << 16 |
                                 song->tracks.at(0).events.at(var).dataByte1 << 8 |
                                 song->tracks.at(0).events.at(var).status));
            event.dwEvent = nvnt;

            evnts[tots] = event;
            tots++;
        }

    }
    int tempNotes[tots*3];
    for (int var = 0; var < tots; ++var) {
      tempNotes[var*3] = evnts[var].dwDeltaTime;
     tempNotes[var*3 + 1] = 0;
     tempNotes[var*3 + 2] = evnts[var].dwEvent;
    }
    unsigned long tnote[] = {0,0,0x007F3C90, /* A note-on */
    0,0,0x007F2D90,10,0,0x007F5A90};
    MIDIHDR buffer;

    buffer.lpData =(LPSTR)&tempNotes;
    buffer.dwBufferLength = sizeof(tempNotes);
    buffer.dwBytesRecorded = sizeof(tempNotes);
    buffer.dwFlags = 0;



 HMIDISTRM outHandle;
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

 result = midiOutPrepareHeader((HMIDIOUT)outHandle,&buffer,sizeof(buffer));
 if (result) {
      qDebug() << "midiOutPrepareHeader error";
 }
 for (int var = 0; var < 10; ++var) {

 }
 result = midiStreamOut(outHandle,&buffer,sizeof(buffer));
 if (result) {
      qDebug() << "midiStreamOut error";
 }
 result = midiStreamRestart(outHandle);
 if (result) {
      qDebug() << "midiStreamRestart error";

 }
  WaitForSingleObject(hEvent, INFINITE);


  midiOutUnprepareHeader((HMIDIOUT)outHandle, &buffer, sizeof(buffer));
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
