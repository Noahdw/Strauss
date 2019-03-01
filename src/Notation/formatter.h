#ifndef FORMATTER_H
#define FORMATTER_H

class Score;

class Formatter
{
public:
    Formatter(Score* _score);
private:
    Score* score;
};

#endif // FORMATTER_H
