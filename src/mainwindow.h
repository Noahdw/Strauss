#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <src/midiplayer.h>
#include <QtConcurrent/QtConcurrent>
#include <src/pianoroll.h>
#include <src/pianorollitem.h>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <src/MidiManager.h>
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget *centralWidget;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QVector<pluginHolder*> pluginHolderVec;
    static bool keyboardModeEnabled;
    static int tempFolderID;
private slots:
    void on_PauseButton_clicked(int type);
    void on_quitButton_clicked();
    void on_actionSave_triggered();
    void on_actionPlay_triggered();
    void on_StartButton_clicked();
    void openFile();
    void playSong();
    void deleteAllNotes();
    void addNewTrack();

private:
    void setUpMenuBar();
    int getNoteFromKeyboard(int key);
    PianoRollContainer     *prollContainer;
    TrackContainer         *trackContainer;
    FolderView             *folderView;
    HeaderContainer        *headerContainer;
    ControlChangeContainer *controlContainer;
    PianoRollHelperView    *prollHelper;

    QMenu   *fileMenu;
    QMenu   *editMenu;
    QAction *saveAction;
    QAction *openFileAction;
    QAction *playSongAction;
    QAction *pauseAction;
    QAction *deleteAllNotesAction;
    QAction *openVSTAction;
    QAction *addNewTrackAction;
    int velocity = 70;
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
