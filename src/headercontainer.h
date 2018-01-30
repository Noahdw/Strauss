#ifndef HEADERCONTAINER_H
#define HEADERCONTAINER_H

//Forward declarations
class AudioManager;

#include <QWidget>
#include <QDebug>
#include <QButtonGroup>
#include <QGroupBox>

class HeaderContainer : public QWidget
{
    Q_OBJECT
public:
    HeaderContainer();
    AudioManager * audioManager;
private:
    QGroupBox * groupBox;
    QButtonGroup * buttonGroup;
    bool isPaused = true;
private slots:
    void play();
    void restart();
    void pause();
};

#endif // HEADERCONTAINER_H
