#include "headercontainer.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <src/audiomanager.h>

const int maxWidth = 50;
HeaderContainer::HeaderContainer()
{
    //buttonGroup = new QButtonGroup;

    auto *playButton    = new QPushButton("Play",this);
    auto *pauseButton   = new QPushButton("Pause",this);
    auto *restartButton = new QPushButton("Restart",this);
    auto *recordButton  = new QPushButton("Record",this);
    auto *hbox          = new QHBoxLayout;
    auto *mainbox       = new QHBoxLayout;
    recordStyle         = new QCheckBox;
    groupBox            = new QGroupBox;

    hbox->addWidget(playButton);
    hbox->addWidget(pauseButton);
    hbox->addWidget(restartButton);
    hbox->addWidget(recordButton);
    hbox->addWidget(recordStyle);
    playButton->setMaximumWidth(maxWidth);
    pauseButton->setMaximumWidth(maxWidth);
    restartButton->setMaximumWidth(maxWidth);
    recordButton->setMaximumWidth(maxWidth);
    hbox->setMargin(0);
    groupBox->setLayout(hbox);
    groupBox->setFlat(true);
    mainbox->addWidget(groupBox);
    mainbox->setAlignment(Qt::AlignTop|Qt::AlignCenter);
    setLayout(mainbox);
    QObject::connect(playButton,   &QPushButton::clicked,this,&HeaderContainer::play);
    QObject::connect(pauseButton,  &QPushButton::clicked,this,&HeaderContainer::pause);
    QObject::connect(restartButton,&QPushButton::clicked,this,&HeaderContainer::restart);
    QObject::connect(recordButton, &QPushButton::clicked,this,&HeaderContainer::record);



    groupBox->setStyleSheet("QGroupBox {  border: 1px solid gray;}");
}

void HeaderContainer::play()
{

    if(isPaused){
        audioManager->requestPauseOrResume(true);
    }
    else{
        audioManager->requestPlaybackRestart();
    }
    isPaused = false;

}

void HeaderContainer::restart()
{
    isPaused = false;
    MidiPlayer::canRecordInput = false;
    audioManager->requestPlaybackRestart();
    if(MidiPlayer::canRecordInput)
    {
           MidiPlayer::addMidiAfterRecording();
           MidiPlayer::canRecordInput = false;
    }
}

void HeaderContainer::pause()
{
    isPaused = true;
    audioManager->requestPauseOrResume(false);
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
