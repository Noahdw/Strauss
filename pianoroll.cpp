#include "pianoroll.h"

PianoRoll::PianoRoll(QWidget * _parent) : QWidget(_parent)
{
    m_contents = new QWidget( this );


}
void PianoRoll::paintEvent(QPaintEvent *){
    QPainter painter(this);
     painter.setPen(Qt::black);

     painter.drawRect(0,0,20,20);
}

void PianoRoll::createPianoRoll(QPaintEvent * event){
   paintEvent(event);

}
