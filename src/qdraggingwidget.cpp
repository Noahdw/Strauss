#include "qdraggingwidget.h"

QDraggingWidget::QDraggingWidget(int min, int max, int defaultValue)
    : min(min),max(max),_value(defaultValue){
    valueLabel = new QLabel(QString::number(defaultValue));
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(valueLabel,0,0,Qt::AlignCenter);
    setLayout(mainLayout);
    setOrientation(Qt::Vertical);
}

QDraggingWidget::QDraggingWidget()
{

}

int QDraggingWidget::getValue()
{
    return _value;
}

void QDraggingWidget::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
}

void QDraggingWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QColor(125,125,125));
    painter.drawRect(0,0,width(),height());
}

void QDraggingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastXPos = event->pos().x();
        lastYPos = event->pos().y();
    }

    QWidget::mousePressEvent(event);
}

void QDraggingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {

        if (orientation == Qt::Vertical)
        {
            if (abs(lastYPos) % 3 == 1 && event->pos().y() != lastYPos)
            {
                int direction = (event->pos().y() > lastYPos) ? -1 : 1;
                if (_value + direction < max && _value + direction > min)
                {
                    _value += direction;
                    emit valueChanged(_value);
                    valueLabel->setText(QString::number(_value));
                }

            }
        }
        else if(orientation == Qt::Horizontal && event->pos().x() != lastXPos)
        {
            int direction = (event->pos().x() > lastXPos) ? -1 : 1;
            if (_value + direction < max && _value + direction > min)
            {
                _value += direction;
                emit valueChanged(_value);
                valueLabel->setText(QString::number(_value));
            }
        }

        lastXPos = event->pos().x();
        lastYPos = event->pos().y();
    }
    QWidget::mouseMoveEvent(event);
}

