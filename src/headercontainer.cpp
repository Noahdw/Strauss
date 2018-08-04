#include "headercontainer.h"
#include <src/audioengine.h>
#include <src/mainwindow.h>


const int maxWidth = 50;
HeaderContainer::HeaderContainer(AudioEngine *audioManager)
{
    audio_manager = audioManager;

    auto *playButton     = new QPushButton("Play",this);
    auto *pauseButton    = new QPushButton("Pause",this);
    auto *restartButton  = new QPushButton("Restart",this);
    auto *recordButton   = new QPushButton("Record",this);
    auto *keyboardButton = new QPushButton("Keyboard mode",this);
    auto *hbox           = new QHBoxLayout;
    auto *mainbox        = new QHBoxLayout;
    recordStyle          = new QCheckBox;
    groupBox             = new QGroupBox;

    hbox->addWidget(playButton);
    hbox->addWidget(pauseButton);
    hbox->addWidget(restartButton);
    hbox->addWidget(recordButton);
    hbox->addWidget(recordStyle);
    hbox->addWidget(keyboardButton);
    playButton->setMaximumWidth(maxWidth);
    pauseButton->setMaximumWidth(maxWidth);
    restartButton->setMaximumWidth(maxWidth);
    recordButton->setMaximumWidth(maxWidth);
    keyboardButton->setCheckable(true);
    hbox->setMargin(0);
    groupBox->setLayout(hbox);
    groupBox->setFlat(true);
    mainbox->addWidget(groupBox);
    mainbox->setAlignment(Qt::AlignTop|Qt::AlignCenter);
    setLayout(mainbox);
    QObject::connect(playButton,     &QPushButton::clicked,this,&HeaderContainer::play);
    QObject::connect(pauseButton,    &QPushButton::clicked,this,&HeaderContainer::pause);
    QObject::connect(restartButton,  &QPushButton::clicked,this,&HeaderContainer::restart);
    QObject::connect(recordButton,   &QPushButton::clicked,this,&HeaderContainer::record);
    QObject::connect(keyboardButton, &QPushButton::toggled,this,&HeaderContainer::keyboardMode);



    groupBox->setStyleSheet("QGroupBox {  border: 1px solid gray;}");
}

void HeaderContainer::play()
{

    if(isPaused){
        audio_manager->requestPauseOrResume(true);
    }
    else{
        audio_manager->requestPlaybackRestart();
    }
    isPaused = false;

}

void HeaderContainer::restart()
{
    isPaused = false;
    MidiPlayer::canRecordInput = false;
    audio_manager->requestPlaybackRestart();
    if(MidiPlayer::canRecordInput)
    {
           MidiPlayer::addMidiAfterRecording();
           MidiPlayer::canRecordInput = false;
    }
}

void HeaderContainer::pause()
{
    isPaused = true;
    audio_manager->requestPauseOrResume(false);
    if(MidiPlayer::canRecordInput)
    {
           MidiPlayer::addMidiAfterRecording();
           MidiPlayer::canRecordInput = false;
    }

}

void HeaderContainer::record()
{
    MidiPlayer::canRecordInput = true;
    MidiPlayer::recordingOverwrites = recordStyle->isChecked();
    play();
}

void HeaderContainer::keyboardMode(bool enabled)
{
    keyboardModeEnabled = enabled;
}
