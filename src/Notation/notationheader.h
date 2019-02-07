#ifndef NOTATIONHEADER_H
#define NOTATIONHEADER_H

class NotationMainWindow;

#include <QObject>
#include <QFrame>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include "src/Notation/n_common.h"
class NotationHeader : public QFrame
{
    Q_OBJECT
public:
    NotationHeader(NotationMainWindow *mainWin);
private:
    NotationMainWindow *mainWindow;
    void changeNoteValue(float value);
};

#endif // NOTATIONHEADER_H
