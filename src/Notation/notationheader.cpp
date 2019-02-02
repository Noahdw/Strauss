#include "notationheader.h"
#include "src/Notation/notationmainwindow.h"

NotationHeader::NotationHeader(NotationMainWindow *mainWin)  : mainWindow(mainWin)
{

    QPushButton *qrtNote = new QPushButton("Qrt note");
    QPushButton *halfNote = new QPushButton("Half note");
    QGroupBox *box = new QGroupBox;
    QHBoxLayout *hLayout = new QHBoxLayout;
    auto *mainLayout = new QHBoxLayout;
    auto * widget = new QWidget;
    hLayout->addWidget(qrtNote);
    hLayout->addWidget(halfNote);
    box->setLayout(hLayout);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
}
