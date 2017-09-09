#include "mainwindow.h"
#include <keyboard.h>
#include <windows.h>
#include <vst2hostcallback.h>
#include <audiomanager.h>
MidiPlayer player;
MidiManager *manager;
PianoRoll *pianoRollView;
TrackView * trackview;
AudioManager* audioManager;

//temp
Vst2HostCallback* host;
AEffect *plugin = NULL;

MainWindow::MainWindow(MidiManager *mngr,QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    pianoRollView = new PianoRoll;
    audioManager = new AudioManager;

    manager = new MidiManager;
    host = new Vst2HostCallback(manager);
    pianoRollView->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    QScrollArea *trackScrollArea = new QScrollArea;
    trackScrollArea->setBackgroundRole(QPalette::Light);
    trackScrollArea->setWidgetResizable(true);
    trackScrollArea->setFixedSize(70,300);
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

    // playSong();

}
QFuture<void> future;
bool stopped = false;

// Starts audio engine if a plugin is active
// restarts a song if pressed while playing
void MainWindow::playSong(){
    

    if (plugin != NULL) {

          if (!audioManager->isRunning) {
              audioManager->sampleRate = host->sampleRate;
              audioManager->blocksize = host->blocksize;
              audioManager->startPortAudio();

              audioManager->openStream();
              audioManager->startStream(host,plugin);
          }
          else
          {
            audioManager->requestPlaybackRestart();
          }


    }

}

void MainWindow::on_actionPlay_triggered()
{
    playSong();
}

void MainWindow::openVST()
{



    QFileDialog dialog;
    QString fileName  = dialog.getOpenFileName(this, tr("Open File"), QString(),
                                               tr("dll Files (*.dll)"));

    if (!fileName.isEmpty()) {
        QByteArray array = fileName.toLocal8Bit();
        char* file = array.data();



        plugin = host->loadPlugin(file);
        if (plugin == NULL) {
            qDebug() << "NULLPTR PLUGIN";
            return;
        }
        int state = host->configurePluginCallbacks(plugin);
        if (state == -1) {
            qDebug() << "Failed to configure button. abort startPlugin";
            return;
        }
        host->startPlugin(plugin);




    }
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


    openVSTAction = new QAction(tr("&Open VST"),this);
    openVSTAction->setStatusTip(tr("Opens a VST"));
    connect(openVSTAction, &QAction::triggered, this, &MainWindow::openVST);
    //Add pause

    //Create menu crap

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(playSongAction);
    fileMenu->addAction(openVSTAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllNotesAction);


}

void MainWindow::connectSlots(PianoRoll*proll,Keyboard *key)
{
    QObject::connect(proll,&PianoRoll::addNoteToPROLL,this,&MainWindow::updatePROLL);
    QObject::connect(key,&Keyboard::playSelectedNote,&player,&MidiPlayer::playNote);

}
