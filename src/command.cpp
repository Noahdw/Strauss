#include "command.h"
#include "pianorollwidget.h"
#include "pianorollitem.h"
#include "trackmidi.h"
#include "velocityview.h"
#include "Controllers/pianorollcontainer.h"

Command::Command()
{

}

Command::~Command()
{

}



PianoRollMoveCommand::PianoRollMoveCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem*> items, int x, int y, QGraphicsItem *skipItem)
    :skippedItem(skipItem), items(items),xPos(x),yPos(y),_pianoRoll(pianoRoll)
{

}

void PianoRollMoveCommand::execute()
{
    foreach (auto note, items) {
        if(skippedItem == note)
            continue;
        note->setX(note->x() + xPos);
        note->setY(note->y() + yPos);
    }

    for(const auto& note : items)
    {
        if (skippedItem == note) continue;

        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,_pianoRoll->midiTrack()->midiData());
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}


void PianoRollMoveCommand::undo()
{
    foreach (auto &note, items) {
        if(skippedItem == note)
            continue;
        note->setX(note->x() - xPos);
        note->setY(note->y() - yPos);
    }

    for(const auto& note : items)
    {
        if (skippedItem == note)
        {
            continue;
        }
        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,_pianoRoll->midiTrack()->midiData());
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}


PianoRollAddCommand::PianoRollAddCommand(PianoRollWidget *pianoRoll, QList<ItemData> items)
    : items(items),_pianoRoll(pianoRoll)
{

}
void PianoRollAddCommand::execute()
{
    for(const auto& item : items)
    {
        PianoRollItem *pNote = new PianoRollItem;
        _pianoRoll->scene()->addItem(pNote);
        pNote->pianoroll = _pianoRoll;
        pNote->setInitalPosition(item.xPos,item.length,item.yPos);
        pNote->setBoundingRect(item.length);
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(item.xPos,70,item.yPos,true);
        MidiManager::addMidiNote(item.yPos,70,item.xPos,item.length,_pianoRoll->midiTrack()->midiData());
        newItems.append(pNote);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}


void PianoRollAddCommand::undo()
{
    foreach (const auto &item, newItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int note = 127 - (pItem->y()/keyHeight);
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pItem->x(),0,note,false);
        MidiManager::removeMidiNote(pItem->x(),pItem->width,note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->scene()->removeItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}

PianoRollAddCommand::~PianoRollAddCommand()
{

}


PianoRollRemoveCommand::PianoRollRemoveCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem *> items)
    : removedItems(items),_pianoRoll(pianoRoll)
{

}

void PianoRollRemoveCommand::execute()
{
    foreach (const auto& item, removedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int note = 127 - (pItem->y()/keyHeight);
        pItem->velocity = MidiManager::getVelocityFromNote(pItem->x(),note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pItem->noteStart,pItem->velocity,note,false);
        MidiManager::removeMidiNote(pItem->x(),pItem->width,note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->scene()->removeItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}

void PianoRollRemoveCommand::undo()
{
    foreach (const auto& item, removedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);

        int note = 127 - (pItem->y()/keyHeight);
        MidiManager::addMidiNote(note,pItem->velocity,pItem->x(),pItem->width,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pItem->x(),pItem->velocity,note,true);
        _pianoRoll->scene()->addItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}

PianoRollResizeCommand::PianoRollResizeCommand(PianoRollWidget *pianoRoll, QList<QGraphicsItem *> items, int xLeftAdjust, int xRightAdjust)
{
    _pianoRoll = pianoRoll;
    resizedItems = items;
    xLeft = xLeftAdjust;
    xRight = xRightAdjust;
}

void PianoRollResizeCommand::execute()
{
    foreach (const auto & item, resizedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int orgigX = pItem->x() - xLeft;
        int origWidth = pItem->width - xRight;
        int note = 127 - (pItem->y()/keyHeight);
        MidiManager::removeMidiNote(orgigX,origWidth,note,_pianoRoll->midiTrack()->midiData());
        MidiManager::addMidiNote(note,pItem->velocity,pItem->x(),pItem->width,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(orgigX,pItem->velocity,note,false);
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pItem->x(),pItem->velocity,note,true);


    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}

void PianoRollResizeCommand::undo()
{
    foreach (const auto & item, resizedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int origX = pItem->x() - xLeft;
        int origWidth = pItem->width - xRight;
        int note = 127 - (pItem->y()/keyHeight);
        MidiManager::removeMidiNote(pItem->x(),pItem->width,note,_pianoRoll->midiTrack()->midiData());
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(pItem->x(),pItem->velocity,note,false);
        _pianoRoll->container()->velocityView()->addOrRemoveVelocityViewItem(origX,pItem->velocity,note,true);
        MidiManager::addMidiNote(note,pItem->velocity,origX,origWidth,_pianoRoll->midiTrack()->midiData());
        pItem->setX(origX);
        pItem->setBoundingRect(origWidth);

    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack()->midiData());
}
