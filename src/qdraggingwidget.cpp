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

const int QDraggingWidget::getValue()
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
    painter.setBrush(QColor(100,100,100));
    painter.drawRect(0,0,width(),height());
}

void QDraggingWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (orientation == Qt::Vertical)
            lastMousePos = event->pos().y();
        if (orientation == Qt::Horizontal)
            lastMousePos = event->pos().x();
    }

    QWidget::mousePressEvent(event);
}

void QDraggingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        if (abs(lastMousePos) % 3 == 1)
        {
            if (orientation == Qt::Vertical)
            {
                int direction = (event->pos().y() > lastMousePos) ? -1 : 1;
                if (_value + direction < max && _value + direction > min)
                {
                    _value += direction;
                    emit valueChanged(_value);
                    valueLabel->setText(QString::number(_value));
                }
            }
            else if(orientation == Qt::Horizontal)
            {
                int direction = (event->pos().x() > lastMousePos) ? -1 : 1;
                if (_value + direction < max && _value + direction > min)
                {
                    _value += direction;
                    emit valueChanged(_value);
                    valueLabel->setText(QString::number(_value));
                }
            }
        }
        if (orientation == Qt::Vertical)
            lastMousePos = event->y();
        if (orientation == Qt::Horizontal)
            lastMousePos = event->x();
    }
    QWidget::mouseMoveEvent(event);
}

