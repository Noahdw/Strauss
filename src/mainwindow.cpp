#include "mainwindow.h"
#include <src/keyboard.h>
#include <windows.h>
#include <src/vst2hostcallback.h>
#include <src/audiomanager.h>
#include <src/midiplayer.h>


MidiPlayer player;
MidiManager *manager;

AudioManager* audioManager;

//temp
Vst2HostCallback* host;
AEffect *plugin = NULL;
bool MainWindow::keyboardModeEnabled = false;
int MainWindow::tempFolderID = 0;
QVector<pluginHolder*> MainWindow::pluginHolderVec;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    audioManager = new AudioManager;
    manager = new MidiManager;


    QScrollArea *trackScrollArea = new QScrollArea;
    trackScrollArea->setBackgroundRole(QPalette::Light);
    trackScrollArea->setWidgetResizable(true);
    trackScrollArea->setMinimumWidth(1000);
    trackScrollArea->setMinimumHeight(300);
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    folderView       = new FolderView;
    headerContainer  = new HeaderContainer;
    trackContainer   = new TrackContainer;
    prollContainer   = new PianoRollContainer;
    controlContainer = new ControlChangeContainer(prollContainer);
    prollHelper      = new PianoRollHelperView;

    trackContainer->setPianoRollReference(prollContainer);
    folderView->pRollContainer = prollContainer;
    headerContainer->audioManager = audioManager;
    trackScrollArea->setWidget(trackContainer);

    QVBoxLayout *mainLayout   = new QVBoxLayout;
    QHBoxLayout *helperLayout = new QHBoxLayout;
    QSplitter *trackSplitter  = new QSplitter;
    QSplitter *prollSplitter  = new QSplitter;

    prollSplitter->setOrientation(Qt::Vertical);
    helperLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    helperLayout->setContentsMargins(0,0,0,0);
    trackSplitter->addWidget(trackScrollArea);
    trackSplitter->addWidget(folderView);
    mainLayout->addWidget(headerContainer);
    prollSplitter->addWidget(trackSplitter);
    prollSplitter->addWidget(controlContainer);
    helperLayout->addWidget(prollHelper);
    helperLayout->addWidget(prollSplitter);
    mainLayout->addLayout(helperLayout);
    prollHelper->container = controlContainer;
    centralWidget->setLayout(mainLayout);

    QObject::connect(trackContainer,&TrackContainer::switchControlChange,controlContainer,
                     &ControlChangeContainer::switchControlChangeContainer);
    QObject::connect(manager,&MidiManager::notifyTrackViewChanged,trackContainer,&TrackContainer::addTrackView);
    addNewTrack();
    setUpMenuBar();
    AudioManager::requestedPlaybackPos = -1;
    audioManager->startPortAudio();
    audioManager->openStream();
    audioManager->startStream();

    int devices = player.getDevices();
    for (int i = 0; i < devices; ++i)
    {
        player.openDevice(i);
    }
    player.openDevice(3);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(150,150,150));
    setAutoFillBackground(true);
    setPalette(pal);


        QDir dir(QDir::current().path()+"/TempPlugins");
        dir.removeRecursively();
        dir.mkdir(QDir::current().path()+"/TempPlugins");
}

MainWindow::~MainWindow()
{

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
        // prollContainer->pianoRoll->convertFileToItems(*manager);

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

    audioManager->requestPlaybackRestart();

}

void MainWindow::on_actionPlay_triggered()
{
    playSong();
}

void MainWindow::deleteAllNotes()
{
    //  prollContainer->pianoRoll->deleteAllNotes();
}

void MainWindow::setUpMenuBar()
{

    //Create action crap
    openFileAction = new QAction(tr("&Open Midi File"),this);
    openFileAction->setStatusTip(tr("Open an existing midi file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    playSongAction = new QAction(tr("&Play"),this);
    playSongAction->setStatusTip(tr("play the current song"));
    connect(playSongAction, &QAction::triggered, this, &MainWindow::playSong);

    deleteAllNotesAction = new QAction(tr("&Delete all"),this);
    deleteAllNotesAction->setStatusTip(tr("Delete all notes from the roll"));
    connect(deleteAllNotesAction, &QAction::triggered, this, &MainWindow::deleteAllNotes);

    addNewTrackAction = new QAction(tr("&Add Track"),this);
    addNewTrackAction->setStatusTip(tr("Adds a new MIDI track"));
    connect(addNewTrackAction, &QAction::triggered, this, &MainWindow::addNewTrack);
    //Add pause

    //Create menu crap
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(playSongAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllNotesAction);
    editMenu->addAction(addNewTrackAction);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (MainWindow::keyboardModeEnabled)
    {
        if (event->isAutoRepeat())
        {
            event->ignore();
            QMainWindow::keyReleaseEvent(event);
            return;
        }
        for(const auto& plugin : pluginHolderVec)
        {
            if (plugin->host->canRecord())
            {
                int note = getNoteFromKeyboard(event->key());
                if (note)
                {
                    plugin->host->addMidiEvent(0x90,note,velocity);
                }
            }
        }
    }
    else
    {
        event->ignore();
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (MainWindow::keyboardModeEnabled)
    {
        if (event->isAutoRepeat())
        {
            event->ignore();
            QMainWindow::keyReleaseEvent(event);
            return;
        }
        for(const auto& plugin : pluginHolderVec)
        {
            if (plugin->host->canRecord())
            {
                int note = getNoteFromKeyboard(event->key());
                if (note)
                {
                   plugin->host->addMidiEvent(0x90,note,0);
                }
            }
        }
    }
    else
    {
        event->ignore();
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::addNewTrack()
{
    mTrack *track = new mTrack;
    track->instrumentName = "New Track";
    TrackView *view = new TrackView(track);
    trackContainer->addSingleView(view);
}

int MainWindow::getNoteFromKeyboard(int key)
{
    switch (key)
    {
    case Qt::Key_A:
        return 60; // Middle C
    case Qt::Key_S:
        return 61;
    case Qt::Key_D:
        return 62;
    case Qt::Key_F:
        return 63;
    case Qt::Key_G:
        return 64;
    case Qt::Key_H:
        return 65;
    case Qt::Key_J:
        return 66;
    case Qt::Key_K:
        return 67;
    case Qt::Key_L:
        return 68;
    case Qt::Key_Semicolon:
        return 69;
    case Qt::Key_Apostrophe:
        return 70;
    case Qt::Key_Return:
        return 71;
    default:
        return 0;
    }
}
