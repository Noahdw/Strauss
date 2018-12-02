#include "headercontainer.h"
#include <src/audioengine.h>
#include <src/mainwindow.h>


const int maxWidth = 50;
HeaderContainer::HeaderContainer(AudioEngine *audioManager)
{

    audio_manager = audioManager;

    auto *playButton     = new QPushButton(this);
    auto *pauseButton    = new QPushButton(this);
    auto *restartButton  = new QPushButton(this);
    auto *recordButton   = new QPushButton("Record",this);
    auto *keyboardButton = new QPushButton("Keyboard mode",this);
    auto *hbox           = new QHBoxLayout;
    auto *mainbox        = new QHBoxLayout;
    recordStyle          = new QCheckBox;
    groupBox             = new QGroupBox;
    auto *tempoLabel     = new QLabel("Tempo");
    auto *tempoWidget = new QDraggingWidget(10,400,120);
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
    tempoWidget->setFixedSize(35,35);

    hbox->setMargin(0);
    groupBox->setLayout(hbox);
    groupBox->setFlat(true);
    mainbox->addWidget(tempoLabel);
    mainbox->addWidget(tempoWidget);
    mainbox->addWidget(groupBox);
    mainbox->setAlignment(Qt::AlignTop|Qt::AlignCenter);
    setLayout(mainbox);

    QObject::connect(playButton,     &QPushButton::clicked,this,&HeaderContainer::play);
    QObject::connect(pauseButton,    &QPushButton::clicked,this,&HeaderContainer::pause);
    QObject::connect(restartButton,  &QPushButton::clicked,this,&HeaderContainer::restart);
    QObject::connect(recordButton,   &QPushButton::clicked,this,&HeaderContainer::record);
    QObject::connect(keyboardButton, &QPushButton::toggled,this,&HeaderContainer::keyboardMode);
    QObject::connect(tempoWidget,    &QDraggingWidget::valueChanged,this,&HeaderContainer::masterTempoChanged);


    groupBox->setStyleSheet("QGroupBox {  border: 1px solid gray; padding: 4px;}");
    //  playButton->setStyleSheet("background-image: url(:/images/icons/play_pause.png);");
    QPixmap pic(":/icons/play_pause.png");
    playButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    pauseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
    restartButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaSeekBackward));
    // playButton->setIconSize(pic.size());

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

void HeaderContainer::masterTempoChanged(int tempo)
{
 g_tempo = tempo;
 g_timer->setDuration(((float)(60.0/g_tempo)*g_quarterNotes*1000));
}


