#include "measure.h"
#include "src/Notation/notationpage.h"
#include "src/Notation/stave.h"
Measure::Measure()
{
    measureLine = new QGraphicsLineItem(0,0,0,100);
}

void Measure::setPage(NotationPage *page)
{
    notationPage = page;
    measureLine->setLine(0,0,0,page->lineSpace * 4);
}

void Measure::setStave(Stave *stv)
{
    stave = stv;
}

void Measure::insertNote(float beat, float value)
{
    if(notes.count(beat)) // a note with same beat already exists
    {

    }
    else
    {
        QList<QGraphicsItem*> newNotes;
        if(value == DEFAULT_STATE)
        {
            auto * newNote = new Note(value);
            newNote->setX(xInScene() + width / 2);
            newNote->setY(stave->yInScene + stave->lineHeight / 2);
            notes[1] = newNote;
            newNotes.append(newNote);
            notationPage->updateNotes(NULL,newNotes);
            return;
        }
        auto * newNote = new Note(value);
        auto oldNote = notes[beat];
        notes[beat] = newNote;
        float totalDuration = 0;
        foreach(const auto &n, notes)
        {
            auto castN = dynamic_cast<Note*>(n.second);
            totalDuration += castN->noteValue;
        }
        float beatsNeeded = tsTop - totalDuration;

        float newVal = value;
        while(newVal != beatsNeeded)
        {
            auto *nn = new Note(newVal);
            notes[beat + newVal] = nn;
            newNotes.append(nn);
            newVal *= 2.0;
        }
        notationPage->updateNotes(oldNote,newNotes);
    }
}

int Measure::xInScene()
{
    return measureLine->x();
}

