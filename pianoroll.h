#ifndef PIANOROLL_H
#define PIANOROLL_H
#include <QWidget>
#include <QPainter>

class PianoRoll  : public QWidget {
Q_OBJECT
public:
    PianoRoll(QWidget * _parent);

    void createPianoRoll(QPaintEvent *);

    protected:
    virtual void paintEvent(QPaintEvent * event);

    private:
        QWidget * m_contents;
};


#endif // PIANOROLL_H
