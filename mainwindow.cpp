#include "mainwindow.h"
#include <keyboard.h>

MidiPlayer player;
MidiManager *manager;
PianoRoll *pianoRollView;
TrackView * trackview;

MainWindow::MainWindow(MidiManager *mngr,QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    pianoRollView = new PianoRoll;

    manager = new MidiManager;
    pianoRollView->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    QScrollArea *trackScrollArea = new QScrollArea;
    trackScrollArea->setBackgroundRole(QPalette::Dark);
    trackScrollArea->setWidgetResizable(true);
    trackScrollArea->setFixedSize(70+10,300);
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);


    trackContainer = new TrackContainer;
    prollContainer = new PianoRollContainer;
    trackContainer->setPianoRollReference(prollContainer);
    trackScrollArea->setWidget(trackContainer);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(trackScrollArea);
    mainLayout->addWidget(prollContainer);
    centralWidget->setLayout(mainLayout);


    setUpMenuBar();
    QObject::connect(prollContainer->pianoRoll,&PianoRoll::addNoteToPROLL,this,&MainWindow::updatePROLL);
    QObject::connect(manager,&MidiManager::notifyTrackViewChanged,trackContainer,&TrackContainer::addTrackView);
    QObject::connect(prollContainer->keyboard,&Keyboard::playSelectedNote,&player,&MidiPlayer::playNote);
    QObject::connect(prollContainer,&PianoRollContainer::connectSignals,this,&MainWindow::connectSlots);

}

MainWindow::~MainWindow()
{
    //  delete ui;
}

void MainWindow::updatePROLL(int x,int y, int width,int start, int length)
{
    manager->updateMidi(127 - y/PianoRollItem::keyHeight,70,start,length);
}


void MainWindow::on_quitButton_clicked()
{
    midiStreamStop(player.outHandle);

    QCoreApplication::quit();
}


//Opens and deserializes a song
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("Midi Files (*.mid)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QString s;

        QByteArray array = manager->ReadMidi(file);
        for (int var = 0; var < array.length(); ++var) {
            qint8 val= array.at(var);
            s.append(QString::number(val) + " ");
        }

        manager->song = manager->Deserialize(array);
        prollContainer->pianoRoll->convertFileToItems(*manager);

        file.close();
    }
}


void MainWindow::on_PauseButton_clicked(int type)
{
    if (type == -1) {
        player.pausePlayBack();
    }
    else
        player.resumePlayBack();

}

void MainWindow::on_StartButton_clicked()
{
    player.resumePlayBack();
}

void MainWindow::on_actionSave_triggered()
{
    playSong();

}
QFuture<void> future;
bool stopped = false;

//Resets song back to DT 0 and continue playing.
//needBreak stops qtConc from running if song is still playing, shouldBreak is a flag
//for midiplayer to reset song pos.
void MainWindow::playSong(){
    qDebug() << player.outHandle;
    if ( player.needBreak) {

        midiOutReset((HMIDIOUT)player.outHandle);
        qDebug() << "stopping playback";
        player.shouldBreak = true;
    }
    else{
        player.needBreak = true;
        qDebug() << "starting playback";
        future = QtConcurrent::run(&player,&MidiPlayer::playMidiFile,manager);
    }
}

void MainWindow::on_actionPlay_triggered()
{
    playSong();
}

void MainWindow::deleteAllNotes()
{
    prollContainer->pianoRoll->deleteAllNotes();
}

void MainWindow::setUpMenuBar()
{

    //Create action crap
    openFileAction = new QAction(tr("&Open"),this);
    openFileAction->setStatusTip(tr("Open an existing midi file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    playSongAction = new QAction(tr("&Play"),this);
    playSongAction->setStatusTip(tr("play the current song"));
    connect(playSongAction, &QAction::triggered, this, &MainWindow::playSong);

    deleteAllNotesAction = new QAction(tr("&Delete all"),this);
    deleteAllNotesAction->setStatusTip(tr("Delete all notes from the roll"));
    connect(deleteAllNotesAction, &QAction::triggered, this, &MainWindow::deleteAllNotes);
    //Add pause

    //Create menu crap

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(playSongAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllNotesAction);


}

void MainWindow::connectSlots(PianoRoll*proll,Keyboard *key)
{
    QObject::connect(proll,&PianoRoll::addNoteToPROLL,this,&MainWindow::updatePROLL);
    QObject::connect(key,&Keyboard::playSelectedNote,&player,&MidiPlayer::playNote);

}
