#include "scoreview.h"
#include <QPaintEvent>
#include "element.h"
#include "score.h"
#include "page.h"
#include "measure.h"
#include "moment.h"
#include "formatter.h"


ScoreView::ScoreView(Score *_score) : score(_score), canvas(_score->pageWidth,_score->pageHeight)
{
    format = std::make_unique<Formatter>(_score);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    canvas.fill(Qt::white);
}

void ScoreView::drawElements(QList<Element *> &elements)
{
    QPainter painter(&canvas);
    for(const auto& e : elements)
    {
        e->draw(&painter);
    }
}

void ScoreView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    for(auto& page : score->pages())
    {
        for(auto& measure : page->measures())
        {
            for (int i = 0; i < score->numStaves(); ++i)
            {
                for(auto& moment : measure->moments(i))
                {
                    moment->draw(&painter);
                }
            }

        }
    }

    painter.drawPixmap(event->rect(),canvas);
}

void ScoreView::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}
