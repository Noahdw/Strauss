#include "stave.h"

Stave::Stave()
{
    setMaximumHeight((numLines - 1) * lineHeight);
    measureLayout = new QHBoxLayout;
    measureLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setLayout(measureLayout);
    measureLayout->setContentsMargins(0,0,0,0);
    measureLayout->setSpacing(0);

    // setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void Stave::assignMeasure(Measure *measure)
{
    measureLayout->addWidget(measure);
  //  measure->fitInView(measure->scene->sceneRect(),Qt::IgnoreAspectRatio);
}

void Stave::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    for (int i = 0;i < numLines; i++) {
        painter.drawLine(0,i * lineHeight,width(),i * lineHeight);
    }
  //  painter.drawLine(0,0,0,(numLines - 1) * lineHeight);
  //  painter.drawLine(width(),0,width(),(numLines - 1) * lineHeight);
  //  painter.drawRect(0,0,width(),height());
}
