#include "mainwindow.h"
#include <src/keyboard.h>
#include <windows.h>
#include <src/vst2hostcallback.h>
#include <src/midiplayer.h>
#include <src/common.h>
#include <src/audioengine.h>
#include <src/audiomanager.h>
MidiPlayer player;
MidiManager *manager;
TimeTracker *timeTracker;


//temp
Vst2HostCallback* host;
AEffect *plugin = NULL;

// init common vars
bool keyboardModeEnabled = false;
double g_tempo = 90;
double g_quarterNotes = 60;
int g_blocksize = 64;
int g_sampleRate = 44100;
double g_volume = 0.666;
QTimeLine *g_timer = new QTimeLine((float)(60.0/(float)g_tempo)*g_quarterNotes*1000);//Song time in ms
//end init

QVector<pluginHolder*> MainWindow::pluginHolderVec;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    pluginEdiorCentralWidget = new QWidget(this);
    stackedCentralWidget = new QStackedWidget;
    audio_engine     = new AudioEngine;
    manager          = new MidiManager;
    timeTracker      = new TimeTracker;

    setCentralWidget(stackedCentralWidget);
    trackScrollArea = new QScrollArea;
    trackScrollArea->setWidgetResizable(true);
    trackScrollArea->setMinimumWidth(1000);
    trackScrollArea->setMinimumHeight(300);
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    folder_view              = new FolderView;
    header_container         = new HeaderContainer(audio_engine);
    piano_roll_container     = new PianoRollContainer;
    plugin_editor_container  = new PluginEditorContainer;
    track_container          = new TrackContainer(plugin_editor_container,piano_roll_container);
    control_change_container = new ControlChangeContainer(piano_roll_container);
    piano_roll_helper        = new PianoRollHelperView(control_change_container);
    piano_roll_container->setControlChangeContainer(control_change_container);
    folder_view->pRollContainer = piano_roll_container;

    trackScrollArea->setWidget(track_container);
    mainLayout   = new QVBoxLayout;
    QHBoxLayout *helperLayout   = new QHBoxLayout;
    QSplitter   *trackSplitter  = new QSplitter;
    QSplitter   *prollSplitter  = new QSplitter;

    prollSplitter->setOrientation(Qt::Vertical);
    helperLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    helperLayout->setContentsMargins(0,0,0,0);
    trackSplitter->addWidget(trackScrollArea);
    trackSplitter->addWidget(folder_view);
    mainLayout->addWidget(header_container);
    prollSplitter->addWidget(trackSplitter);
    prollSplitter->addWidget(control_change_container);
    helperLayout->addWidget(piano_roll_helper);
    helperLayout->addWidget(prollSplitter);
    mainLayout->addLayout(helperLayout);
    centralWidget->setLayout(mainLayout);
    stackedCentralWidget->addWidget(centralWidget);
    stackedCentralWidget->addWidget(plugin_editor_container);
    QObject::connect(track_container,&TrackContainer::switchControlChange,control_change_container,
                     &ControlChangeContainer::switchControlChangeContainer);


    addNewTrack();
    setUpMenuBar();
    AudioEngine::requestedPlaybackPos = -1;
    audio_engine->startPortAudio();
    audio_engine->openStream();
    audio_engine->startStream();

    int devices = player.getDevices();
    for (int i = 0; i < devices; ++i)
    {
        player.openDevice(i);
    }
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(150,150,150));
    setAutoFillBackground(true);
    setPalette(pal);


    QDir dir(QDir::current().path()+"/TempPlugins");
    dir.removeRecursively();
    dir.mkdir(QDir::current().path()+"/TempPlugins");

    trackScrollArea->setStyleSheet("QScrollArea {background-color: rgb(170,170,170); border: 2px solid grey }");


}

MainWindow::~MainWindow()
{

}


void MainWindow::on_quitButton_clicked()
{
    midiStreamStop(player.outHandle);

    QCoreApplication::quit();
}


//Opens a midi file and deserializes into a song struct // the song struct is kinda deprecated
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
    track_container->addTrackView(manager->song);
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
void MainWindow::playSong()
{
    audio_engine->requestPlaybackRestart();
}

void MainWindow::saveProject()
{

}

void MainWindow::saveAsProject()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Save as"),QDir::currentPath(),tr("format (*.mipr)"));
    ProjectManager project;
    project.saveAsProject(filePath,*track_container);
}

void MainWindow::loadProject()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open project"), QDir::currentPath(),
                                                    tr("Project files (*.mipr)"));
    ProjectManager project;
    project.loadProject(filePath,this,*track_container);
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

    exportAudioAction = new QAction(tr("&Export Audio"),this);
    exportAudioAction->setStatusTip(tr("Exports as mp3"));
    connect(exportAudioAction, &QAction::triggered, this, &MainWindow::exportAudio);

    saveAction = new QAction("Save project",this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);

    saveAsAction = new QAction("Save as",this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsProject);

    loadAction = new QAction("Load project",this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);

    deleteAllNotesAction = new QAction(tr("&Delete all"),this);
    deleteAllNotesAction->setStatusTip(tr("Delete all notes from the roll"));
    connect(deleteAllNotesAction, &QAction::triggered, this, &MainWindow::deleteAllNotes);

    addNewTrackAction = new QAction(tr("&Add Track"),this);
    addNewTrackAction->setStatusTip(tr("Adds a new MIDI track"));
    connect(addNewTrackAction, &QAction::triggered, this, &MainWindow::addNewTrack);

    settingsAction = new QAction(tr("Settings"),this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::displaySettingsDialog);

    //Add pause

    //Create menu crap
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exportAudioAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(loadAction);

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));
    toolMenu->addAction(settingsAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllNotesAction);
    editMenu->addAction(addNewTrackAction);
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (keyboardModeEnabled)
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
                    plugin->host->addMidiEvent(0x90,note,velocity,0);
                }
            }
        }
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_Alt:
            stackedCentralWidget->setCurrentIndex(!stackedCentralWidget->currentIndex());
            break;
        default:
            break;
        }
        event->ignore();
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (keyboardModeEnabled)
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
                    plugin->host->addMidiEvent(0x90,note,0,0);
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
    track_container->addSingleView();
}

void MainWindow::displaySettingsDialog()
{
    auto settingsDialog = new SettingsDialog(audio_engine);
    settingsDialog->exec();
}

void MainWindow::exportAudio()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Export audio"),QDir::currentPath(),tr("format (*.wav)"));
    audio_engine->stopPortAudio();
    AudioManager audio;
    if (!audio.exportAudio(filePath))
    {
        qDebug() << "Could not export audio";
    }
    audio_engine->startPortAudio();
    audio_engine->openStream();
    audio_engine->startStream();
    // audio_engine->requestPauseOrResume(false);
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
