#ifndef HEADERCONTAINER_H
#define HEADERCONTAINER_H

//Forward declarations
class AudioEngine;

#include <QWidget>
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <src/common.h>
#include "src/qdraggingwidget.h"
class HeaderContainer : public QWidget
{
    Q_OBJECT
public:
    HeaderContainer(AudioEngine *audioManager);

private:
    AudioEngine *audio_manager;
    QGroupBox * groupBox;
    QCheckBox * recordStyle;
    bool isPaused = false;
private slots:
    void play();
    void restart();
    void pause();
    void record();
    void notation();
    void keyboardMode(bool enabled);
    void masterTempoChanged(int tempo);
};

#endif // HEADERCONTAINER_H


