#ifndef HEADERCONTAINER_H
#define HEADERCONTAINER_H

//Forward declarations
class AudioEngine;

#include <QWidget>
#include <QDebug>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <src/common.h>

class HeaderContainer : public QWidget
{
    Q_OBJECT
public:
    HeaderContainer(AudioEngine *audioManager);

private:
    AudioEngine *audio_manager;
    QGroupBox * groupBox;
    QButtonGroup * buttonGroup;
    QCheckBox * recordStyle;
    bool isPaused = false;
private slots:
    void play();
    void restart();
    void pause();
    void record();
    void keyboardMode(bool enabled);
};

#endif // HEADERCONTAINER_H
