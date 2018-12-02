#ifndef QDRAGGINGWIDGET_H
#define QDRAGGINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

class QDraggingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDraggingWidget(int min, int max,int defaultValue);
    QDraggingWidget();
    const int getValue();
    void setMinimumValue(int min);
    void setMaximumValue(int max);
    void setDefaultValue(int defaultValue);
    void setOrientation(Qt::Orientation orientation);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    int valueChanged(int value);
public slots:

private:
    int _value;
    Qt::Orientation orientation;
    int min, max;
    int lastMousePos;
    QLabel *valueLabel;
};

#endif // QDRAGGINGWIDGET_H
