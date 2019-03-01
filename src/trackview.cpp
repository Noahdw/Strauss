#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <src/pianorollitem.h>
#include <QDebug>
#include <src/mainwindow.h>
#include <random>
#include <src/trackmidiview.h>
#include "src/plugintrackview.h"
#include "src/trackmidi.h"
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(80,255);

TrackView::TrackView(TrackMidi *midiTrack, TrackMidiView *trackMidiView,TrackContainer *trackContainer, QWidget *parent) : QFrame(parent)
{
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    _trackContainer = trackContainer;
    _midiTrack = midiTrack;
    _trackMidiView = trackMidiView;
    setMinimumWidth(widgetWidth);
    setMaximumWidth(widgetWidth);
    setMinimumHeight(115);
    setMaximumHeight(115);

    instrumentLabel = new QLineEdit(midiTrack->midiData()->instrumentName);
    instrumentLabel->setReadOnly(true);
    instrumentLabel->setMaximumWidth(widgetWidth-10);
    instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
    instrumentLabel->setFrame(false);
    instrumentLabel->installEventFilter(this);
    instrumentLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    if (instrumentLabel->text() == "")
    {
        instrumentLabel->setText("new track");
    }
    muteButton    = new QPushButton("M",this);
    recordButton  = new QPushButton("R",this);
    auto* hlayoutLevel1 = new QHBoxLayout;
    showButton = new QPushButton("Show",this);
    comboBox = new QComboBox;
    recordButton->setCheckable(true);
    muteButton->setCheckable(true);
    QVBoxLayout *vlayout = new QVBoxLayout;


    vlayout->setContentsMargins(10,5,10,5);
    vlayout->setAlignment(Qt::AlignTop);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    hlayoutLevel1->addWidget(muteButton);
    hlayoutLevel1->addWidget(recordButton);
    vlayout->addLayout(hlayoutLevel1);

    vlayout->addWidget(showButton);
    vlayout->addWidget(comboBox);

    setLayout(vlayout);

    randomRed = distribution(generator);
    randomGreen = distribution(generator);
    randomBlue = distribution(generator);

    connect(instrumentLabel,      &QLineEdit::customContextMenuRequested,this,&TrackView::ShowContextMenu);
    QObject::connect(recordButton,&QPushButton::toggled,   this,&TrackView::toggleRecording);
    QObject::connect(muteButton,  &QPushButton::toggled,   this,&TrackView::toggleMute);
    QObject::connect(showButton,  &QPushButton::clicked,   this,&TrackView::showPlugin);
    QObject::connect(this, &QWidget::customContextMenuRequested,
                     this,&TrackView::ShowContextMenu);
    connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index){ comboBoxIdChanged(index);}); // wtf

    brush = QBrush(QColor(randomRed, randomGreen, randomBlue));
    comboBox->addItem("None");
    comboBox->setMaxCount(24);
}

TrackView::~TrackView()
{
    pluginTrack->deleteLater();

    _trackContainer->deleteTrack(this,_trackMidiView);
}

bool TrackView::eventFilter(QObject *target, QEvent *event)
{
    if(target == instrumentLabel)
    {
        if (event->type() == QEvent::FocusOut)
        {
            instrumentLabel->setReadOnly(true);
            instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
            canEditLine = false;
            instrumentLabel->clearFocus();
            pluginTrack->setTrackName(instrumentLabel->text());
            return false;
        }
        if (canEditLine)
        {
            auto keyPress = dynamic_cast<QKeyEvent*>(event);
            if (keyPress && ((keyPress->key() == Qt::Key_Enter) || (keyPress->key() == Qt::Key_Return)))
            {
                instrumentLabel->setReadOnly(true);
                instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
                canEditLine = false;
                instrumentLabel->clearFocus();
            }
            return false;
        }
        if (event->type() == QEvent::MouseButtonPress )
        {
            if (canEditLine)
            {
                return false;
            }
            else
            {
                setProperty("clicked",true);
                style()->unpolish(this);
                style()->polish(this);
                _trackContainer->trackClicked(this);
                return true;
            }
        }

    }

    return false;
    // return TrackView::eventFilter(target,event);
}


// Will result in this class gettting deleted.
void TrackView::deleteTrack()
{
    _midiTrack->prepareToDelete();

}

void TrackView::deselect()
{
    setProperty("clicked",false);
    style()->unpolish(this);
    style()->polish(this);
}

QString TrackView::getTrackName() const
{
    return instrumentLabel->text();
}

TrackMidiView *TrackView::getTrackMidiView()
{
    return _trackMidiView;
}

void TrackView::toggleMute(bool state)
{
    muteButton->setProperty("toggled",state);
    style()->unpolish(muteButton);
    style()->polish(muteButton);
    midiTrack()->setMuted(state);

}

void TrackView::toggleRecording(bool state)
{
    recordButton->setProperty("toggled",state);
    style()->unpolish(recordButton);
    style()->polish(recordButton);
    midiTrack()->setCanRecord(state);

}

void TrackView::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(("Context menu"), this);
    QAction renameAction("Rename", this);
    QAction deleteAction("Delete track",this);
    connect(&renameAction,&QAction::triggered,this,&TrackView::renameTrack);
    connect(&deleteAction,&QAction::triggered,this,&TrackView::deleteTrack);

    contextMenu.addAction(&renameAction);
    contextMenu.addAction(&deleteAction);
    contextMenu.exec(mapToGlobal(pos));
}

void TrackView::showPlugin()
{
    if(midiTrack()->plugin())
        midiTrack()->plugin()->showPlugin();

}

void TrackView::renameTrack()
{
    instrumentLabel->setReadOnly(false);
    canEditLine = true;
    instrumentLabel->setFocus();
    instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(randomRed, 50, 50, 255); }");//?? works though
}

void TrackView::comboBoxIdChanged(int index)
{

}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(brush);
    painter.drawRect(0,0,width() - 1,instrumentLabel->height() + 7);
}

void TrackView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        ShowContextMenu(event->pos());
    }
    setProperty("clicked",true);
    _trackContainer->trackClicked(this);
    style()->unpolish(this);
    style()->polish(this);
    QFrame::mousePressEvent(event);
}





