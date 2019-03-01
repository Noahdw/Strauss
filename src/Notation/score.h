#ifndef SCORE_H
#define SCORE_H

#include <QObject>
class Stave;
class Measure;
class Page;

struct PageSettings
{
    int lBorderWidth;
    int rBorderWidth;
    int tBorderHeight;
    int bBorderHeight;
};

class Score
{
public:
    Score();
    PageSettings &pageSettings(){return _pageSettings;};
    int numStaves() const {return _staves.size();}
    QList<std::shared_ptr<Page>>& pages() {return _pages;}
    QList<Stave>& staves() {return _staves;}
    QList<std::shared_ptr<Measure>>& measures() {return _measure;}
    void addStave(int index);
    void addMeasure(int index);
    void addPage(int index);
    int pageWidth = 5100;
    int pageHeight = 6600;

private:
    PageSettings _pageSettings;
    QList<Stave> _staves;
    QList<std::shared_ptr<Measure>> _measure;
    QList<std::shared_ptr<Page>> _pages;
};

#endif // SCORE_H
