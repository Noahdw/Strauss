#ifndef NOTATIONHEADER_H
#define NOTATIONHEADER_H

class NotationMainWindow;

#include <QObject>
#include <QFrame>

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
class NotationHeader : public QFrame
{
    Q_OBJECT
public:
    NotationHeader(NotationMainWindow *mainWin);
private:
    NotationMainWindow *mainWindow;
};

#endif // NOTATIONHEADER_H
