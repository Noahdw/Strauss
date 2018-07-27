#ifndef HEADERCONTAINER_H
#define HEADERCONTAINER_H

//Forward declarations
class AudioManager;

#include <QWidget>
#include <QDebug>
#include <QButtonGroup>
#include <QGroupBox>
#include <QCheckBox>

class HeaderContainer : public QWidget
{
    Q_OBJECT
public:
    HeaderContainer();
    AudioManager * audioManager;
private:
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
