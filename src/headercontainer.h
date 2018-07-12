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
    bool isPaused = true;
private slots:
    void play();
    void restart();
    void pause();
    void record();
};

#endif // HEADERCONTAINER_H
