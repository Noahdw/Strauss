#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class AudioManager;

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QUrl>
#include <src/midiplayer.h>
#include <QtConcurrent/QtConcurrent>
#include <src/pianoroll.h>
#include <src/pianorollitem.h>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <src/midimanager.h>
#include <QGraphicsScene>
#include <src/pianorollcontainer.h>
#include <src/trackcontainer.h>
#include <src/folderview.h>
#include <src/headercontainer.h>
#include <QLibrary>
#include <qvector.h>
#include <QSplitter>
#include <src/controlchangecontainer.h>
#include "src/pianorollhelperview.h"
#include <QPainter>
#include <QKeyEvent>
#include <src/timetracker.h>
#include <QStackedWidget>
#include "src/plugineditorcontainer.h"
#include "src/settingsdialog.h"
#include "src/mastertrack.h"
#include "src/projectmanager.h"
#include "src/trackmidi.h"
#include "src/Notation/notationmainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   // static QVector<Vst2HostCallback*> pluginHolderVec;
    AudioEngine *audio_engine;
    MasterTrack *masterTrack;
    MidiPlayer *player;
public slots:
    void addNewTrack();

private slots:
    void on_PauseButton_clicked(int type);
    void on_quitButton_clicked();
    void on_actionSave_triggered();
    void on_actionPlay_triggered();
    void on_StartButton_clicked();
    void openFile();
    void playSong();
    void saveProject();
    void saveAsProject();
    void deleteAllNotes();
    void acceptSettingsDialog(int accept);
    void displaySettingsDialog();
    void switchNotationView();
    void loadProject();
    void exportAudio();
private:
    void setUpMenuBar();
    int getNoteFromKeyboard(int key);

    PianoRollContainer     *piano_roll_container;
    TrackContainer         *track_container;
    FolderView             *folder_view;
    HeaderContainer        *header_container;
    ControlChangeContainer *control_change_container;
    PianoRollHelperView    *piano_roll_helper;
    QVBoxLayout            *mainLayout;
    PluginEditorContainer  *plugin_editor_container;

    QMenu   *fileMenu;
    QMenu   *editMenu;
    QAction *openFileAction;
    QAction *exportAudioAction;
    QAction *pauseAction;
    QAction *deleteAllNotesAction;
    QAction *openVSTAction;
    QAction *addNewTrackAction;
    QAction *settingsAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *loadAction;
    QAction *switchNotationAction;
    QScrollArea *trackScrollArea;
    int velocity = 70;
    QWidget *centralWidget;
    QWidget *pluginEdiorCentralWidget;
    NotationMainWindow *centralNotationWidget;
    QStackedWidget * stackedCentralWidget;
    FolderViewAbstractModel *model;
    QList<QString> getFoldersFromSettings();
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
