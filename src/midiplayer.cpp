#include "midiplayer.h"
#include "src/mainwindow.h"
#include "src/vst2hostcallback.h"
#include "src/controlchangebridge.h"
#include "src/common.h"

MidiPlayer::MidiPlayer()
{

}
uint DeviceID = 1;
void CALLBACK midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);


HANDLE hEvent;
HMIDIIN hMidiDevice = NULL;
bool MidiPlayer::canRecordInput = false;
bool MidiPlayer::recordingOverwrites = false;
//HMIDISTRM outHandle;


void MidiPlayer::playMidiFile(MidiManager *manager)
{
    //    MIDIPROPTIMEDIV prop;
    //    uint32_t result;

    //    hEvent = CreateEvent(0, FALSE, FALSE, 0);
    //    if (hEvent==NULL) {
    //        qDebug() << "Could not creat event";
    //    }

    //    result = midiStreamOpen(&outHandle, &DeviceID, 1,(DWORD)midiCallback, 0, CALLBACK_FUNCTION );
    //    if (result!=MMSYSERR_NOERROR){
    //        qDebug() << "COULD NOT OPEN STREAM1";
    //    }


    //    prop.cbStruct = sizeof(MIDIPROPTIMEDIV);
    //    prop.dwTimeDiv =manager->song.ticksPerQuarterNote;

    //    midiStreamProperty(outHandle, (LPBYTE)&prop, MIDIPROP_SET|MIDIPROP_TIMEDIV);

    //    MIDIHDR buffer;
    //    MIDIHDR buffer1;

    //    const int tpqn = manager->song.ticksPerQuarterNote;
    //    int totalTicks = 0;

    //    std::vector<int> v1;
    //    for (int var = 1; var < manager->noteVec.length()-2; var+=3) {
    //        //Used to put song back at beginning and continue playing
    //        if (shouldBreak) {
    //            qDebug() << "breaking";
    //            shouldBreak = false;
    //            needBreak = true;
    //            var = 1;
    //            totalTicks = 0;
    //            v1.clear();
    //        }


    //        totalTicks +=  manager->noteVec.at(var);
    //        if (totalTicks < tpqn) {
    //            v1.push_back(manager->noteVec.at(var));
    //            v1.push_back(manager->noteVec.at(var + 1));
    //            v1.push_back(manager->noteVec.at(var + 2));
    //        }
    //        else {
    //            buffer1.lpData =(LPSTR)&v1[0];
    //            buffer1.dwBufferLength = sizeof(v1[0]) * v1.size();
    //            buffer1.dwBytesRecorded = sizeof(v1[0]) * v1.size();
    //            buffer1.dwFlags = 0;


    //            buffer = buffer1;
    //            result = midiOutPrepareHeader((HMIDIOUT)outHandle,&buffer,sizeof(buffer));
    //            if (result) {
    //                qDebug() << "midiOutPrepareHeader error:" << result;
    //            }

    //            result = midiStreamOut(outHandle,&buffer,sizeof(buffer));
    //            if (result) {
    //                qDebug() << "midiStreamOut error:" << result;
    //            }
    //            result = midiStreamRestart(outHandle);
    //            if (result) {
    //                qDebug() << "midiStreamRestart error";

    //            }


    //            WaitForSingleObject(hEvent, INFINITE);
    //            v1.clear();
    //            if ((var + 3) >= manager->noteVec.length()) {
    //                break;
    //            }
    //            v1.push_back(manager->noteVec.at(var));
    //            v1.push_back(manager->noteVec.at(var + 1));
    //            v1.push_back(manager->noteVec.at(var + 2));
    //            totalTicks = 0;
    //        }




    //        midiOutUnprepareHeader((HMIDIOUT)outHandle, &buffer, sizeof(buffer));
    //    }
    //    midiStreamClose(outHandle);
    //    CloseHandle(hEvent);
    //    needBreak = false;
}

void MidiPlayer::openDevice(uint deviceNumber)
{
    MMRESULT res;
    if ((res = midiInOpen(&hMidiDevice,deviceNumber,(DWORD_PTR)midiCallback,0,CALLBACK_FUNCTION )) != MMSYSERR_NOERROR )
    {
        qDebug() << "Error opening device: " << res;
        return;
    }
    midiInStart(hMidiDevice);
}

int MidiPlayer::getDevices()
{
    MIDIINCAPSA cap;
    UINT_PTR numDevs = midiInGetNumDevs();
    qDebug() << "Number of devs: " << numDevs;
    for (uint i = 0; i < numDevs; i++)
    {
        MMRESULT  res;
        if(!midiInGetDevCapsA(i, &cap,sizeof(cap)))
        {
            qDebug() << i << " " << cap.szPname;
        }

    }
    return numDevs;
}

// Midi is added after the user is finished recording. This means neither the piano roll or the
// structure that holds midi data are altered until recording stops.
void MidiPlayer::addMidiAfterRecording()
{
    for (int var = 0; var < MainWindow::pluginHolderVec.length() ; ++var)
    {
        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        if (!plugs->host->recordedMidiEventDeque.empty())
        {
            while(!plugs->host->recordedMidiEventDeque.empty())
            {
                EventToAdd event = plugs->host->recordedMidiEventDeque.front();
                plugs->host->recordedMidiEventDeque.pop_front();
                if (event.status != 0x90 || event.velocity == 0)
                {
                    if (event.status == 0xB0)
                    {
                        plugs->host->pianoroll->bridge->verifyOverlayExists(event.note);
                        plugs->host->pianoroll->bridge->overlays[event.note]->addPoint(event.timeInTicks,event.velocity);
                    }
                    continue;
                }
                for(const auto& e : plugs->host->recordedMidiEventDeque)
                {
                    if (e.note == event.note)
                    {
                        int length = e.timeInTicks - event.timeInTicks;
                        MidiManager::addMidiNote(event.note,event.velocity,event.timeInTicks,length,plugs->host->track);
                          qDebug() << "ADDING MIDI. NOTE: " << event.note << " VELOCITY: " << event.velocity << " TotalTime: " << event.timeInTicks
                                   << " Length: " << length;
                          plugs->host->pianoroll->addNoteToScene(event.note,event.timeInTicks,length,event.velocity);
                        break;
                    }
                }
            //    MidiManager::addUnfinishedNote(event.note,event.velocity,event.timeInTicks,plugs->host->track);
            }
            MidiManager::recalculateNoteListDT(plugs->host->track);
            for (int i = 0; i < 128; ++i)
            {
                if (plugs->host->pianoroll->bridge->overlays[i] != NULL)
                {
                    plugs->host->pianoroll->bridge->overlays[i]->recalculateDT();
                }
            }

        }
    }
}

//taken from somewhere, idk.
void CALLBACK midiCallback(HMIDIIN  handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    uchar status = dwParam1 & 0xFF;
    uchar note = dwParam1 >> 8;
    uchar velocity = dwParam1 >> 16;
    if (status >= 0xB0 && status <=0xBF)
    {
        status = 0xB0;
    }
  //  qDebug() << "status: " << status << " Note: " << note << " Velocity: " << velocity;
    if (status == 0x90 || status == 0xB0) // Note on / Note off
    {
        for (int var = 0; var < MainWindow::pluginHolderVec.length() ; ++var)
        {
            pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
            if(plugs->host->canRecord()){
                qreal currentTick =((qreal)g_timer->currentTime() / 1000.0) * 960.0 / (60.0 / g_tempo);
                plugs->host->addMidiEvent(status,note,velocity,currentTick);
                if (MidiPlayer::canRecordInput)
                {

                    qDebug() << currentTick;
                    if (MidiPlayer::recordingOverwrites) // not implemented yet
                    {
                        plugs->host->recordedMidiEventDeque.push_back(EventToAdd{status,note,false,false,currentTick,velocity});
                    }else
                    {
                        plugs->host->recordedMidiEventDeque.push_back(EventToAdd{status,note,false,false,currentTick,velocity});

                    }
                }

            }
        }
        qDebug() << "status: " << status << " Note: " << note << " Velocity: " << velocity;
    }
}
bool streamOpen = false;
void MidiPlayer::pausePlayBack(){
    midiStreamPause(outHandle);
}
//This allows for playback from the piano roll keyboard
void MidiPlayer::Midiman(int note,bool active){


    //    uchar noteTT = note;
    //    uchar velocity = 80;
    //    DWORD inote;
    //    uchar status = 0x90;
    //    if (!active) {

    //        velocity = 0;
    //        status = 0x80;

    //    }
    //    inote =( velocity << 16 |
    //             noteTT << 8 |
    //             status);
    //    int result;
    //    std::vector<int> v1;
    //    qDebug() << "PLAYNOTE(): " <<  inote;
    //    //int notes[] = {0, 0, 0x007F3C90};
    //    v1.push_back(0);
    //    v1.push_back(0);
    //    v1.push_back(inote);

    //    hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    //    if (hEvent==nullptr) {
    //        qDebug() << "CCould not creat event";
    //    }
    //    if (!streamOpen) {


    //        result = midiStreamOpen(&outHandle, &DeviceID, 1,(DWORD)midiCallback, 0, CALLBACK_FUNCTION );
    //        if (result!=MMSYSERR_NOERROR){
    //            qDebug() << "CCOULD NOT OPEN STREAM?";
    //        }
    //        else{streamOpen = true;}
    //    }
    //    MIDIHDR buffer;
    //    buffer.lpData =(LPSTR)&v1[0];
    //    buffer.dwBufferLength = sizeof(v1[0]) * v1.size();
    //    buffer.dwBytesRecorded = sizeof(v1[0]) * v1.size();
    //    buffer.dwFlags = 0;
    //    result = midiOutPrepareHeader((HMIDIOUT)outHandle,&buffer,sizeof(buffer));
    //    result = midiStreamOut(outHandle,&buffer,sizeof(buffer));
    //    if (result) {
    //        qDebug() << "midiStreamOut playNote() error:" << result;
    //    }
    //    result = midiStreamRestart(outHandle);
    //    if (result) {
    //        qDebug() << "midiStreamRestart playNote() error";

    //    }

    //    WaitForSingleObject(hEvent, INFINITE);
    //    midiOutUnprepareHeader((HMIDIOUT)outHandle, &buffer, sizeof(buffer));

    //midiStreamClose(outHandle);
    //streamOpen = false;
    //CloseHandle(hEvent);

}

void MidiPlayer::resumePlayBack()
{
    int result = midiStreamRestart(outHandle);
    if (result) {
        qDebug() << "resumePlayBack error";

    }
}

//Simply calls Midiman for playback - perhaps not needed but seems to be for threading
void MidiPlayer::playNote(int note,bool active)
{
    QFuture<void> future = QtConcurrent::run(this,&MidiPlayer::Midiman,note,active);

}
