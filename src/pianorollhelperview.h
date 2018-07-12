#ifndef PIANOROLLHELPERVIEW_H
#define PIANOROLLHELPERVIEW_H

class ControlChangeContainer;

#include <QFrame>
#include <QObject>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QComboBox>

class PianoRollHelperView : public QFrame
{
    Q_OBJECT
public:
    PianoRollHelperView();
    QComboBox *comboBox;
    ControlChangeContainer *container;
public slots:
    void comboBoxIdChanged(int index);
private:
    QVBoxLayout *vLayout;
    void initializeComboBox();
protected:
   void paintEvent(QPaintEvent *event);
};

#endif // PIANOROLLHELPERVIEW_H
