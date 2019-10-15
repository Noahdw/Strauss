#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
class PianoRollWidget;

struct ItemData{
    int xPos;
    int yPos;
    int length;
};

class Command
{
public:
    Command();
    ~Command();
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class PianoRollMoveCommand : Command
{
public:
    PianoRollMoveCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem*> items, int x, int y, QGraphicsItem *skipItem);
    ~PianoRollMoveCommand();
    void execute();
    void undo();
private:
    QList<QGraphicsItem*> items;
    QGraphicsItem *skippedItem;
    int xPos;
    int yPos;
    PianoRollWidget* _pianoRoll;

};

class PianoRollAddCommand : Command
{
public:
    PianoRollAddCommand(PianoRollWidget *pianoRoll, QList<ItemData> items);
    ~PianoRollAddCommand();
    void execute();
    void undo();

private:
    QList<QGraphicsItem*> newItems;
    QList<ItemData> items;
    PianoRollWidget* _pianoRoll;

};

class PianoRollRemoveCommand : Command
{
public:
    PianoRollRemoveCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem*> items);
    ~PianoRollRemoveCommand();
    void execute();
    void undo();

private:
    QList<QGraphicsItem*> removedItems;
    PianoRollWidget* _pianoRoll;

};

class PianoRollResizeCommand : Command
{
public:
    PianoRollResizeCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem*> items, int xLeftAdjust, int xRightAdjust);
    ~PianoRollResizeCommand();
    void execute();
    void undo();

private:
    QList<QGraphicsItem*> resizedItems;
    int xLeft;
    int xRight;
    PianoRollWidget* _pianoRoll;

};

#endif // COMMAND_H
