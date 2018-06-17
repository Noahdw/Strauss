#include "headercontainer.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <src/audiomanager.h>

const int maxWidth = 50;
HeaderContainer::HeaderContainer()
{
    //buttonGroup = new QButtonGroup;
    groupBox    = new QGroupBox;

    auto *playButton    = new QPushButton("Play",this);
    auto *pauseButton   = new QPushButton("Pause",this);
    auto *restartButton = new QPushButton("restart",this);
    auto *hbox          = new QHBoxLayout;
    auto *mainbox       = new QHBoxLayout;

    hbox->addWidget(playButton);
    hbox->addWidget(pauseButton);
    hbox->addWidget(restartButton);

    playButton->setMaximumWidth(maxWidth);
    pauseButton->setMaximumWidth(maxWidth);
    restartButton->setMaximumWidth(maxWidth);
    hbox->setMargin(0);
    groupBox->setLayout(hbox);
    groupBox->setFlat(false);
    mainbox->addWidget(groupBox);
    mainbox->setAlignment(Qt::AlignTop|Qt::AlignCenter);
    setLayout(mainbox);

    QObject::connect(playButton,&QPushButton::clicked,this,&HeaderContainer::play);
    QObject::connect(pauseButton,&QPushButton::clicked,this,&HeaderContainer::pause);
    QObject::connect(restartButton,&QPushButton::clicked,this,&HeaderContainer::restart);

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
    audioManager->requestPlaybackRestart();
}

void HeaderContainer::pause()
{
    isPaused = true;
    audioManager->requestPauseOrResume(false);
}
