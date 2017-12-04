#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <src/midiplayer.h>
#include<QtConcurrent/QtConcurrent>
#include <src/pianoroll.h>
#include <src/pianorollitem.h>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <src/MidiManager.h>
#include <QGraphicsScene>
#include <src/pianorollcontainer.h>
#include <src/trackcontainer.h>
#include <QLibrary>
#include <qvector.h>
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

private slots:
    void on_quitButton_clicked();

    void openFile();

    void on_actionSave_triggered();

    void on_PauseButton_clicked(int type);

    void on_StartButton_clicked();

    void playSong();

    void updatePROLL(int x,int y,int width,int start, int length);

    void on_actionPlay_triggered();

    void openVST();

    void deleteAllNotes();

    void addNewTrack();

private:
   PianoRollContainer *prollContainer;
   TrackContainer *trackContainer;

    QMenu *fileMenu;
    QMenu *editMenu;
    QAction *saveAction;
    QAction *openFileAction;
    QAction *playSongAction;
    QAction *pauseAction;
    QAction *deleteAllNotesAction;
    QAction *openVSTAction;
    QAction *addNewTrackAction;

    void setUpMenuBar();
};

#endif // MAINWINDOW_H
