#ifndef SCOREVIEW_H
#define SCOREVIEW_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPainter>

class Formatter;
class Score;
class Element;

class ScoreView : public QWidget
{
public:
    ScoreView(Score *score);
    void drawElements(QList<Element*> &elements);
    bool noteEntry = true;
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
private:
    Score* score;
    QPixmap canvas;
    std::unique_ptr<Formatter> format;

};

#endif // SCOREVIEW_H
