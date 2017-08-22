#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <midimanager.h>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <midiplayer.h>
#include<QtConcurrent/QtConcurrent>
#include <pianoroll.h>
#include <pianorollitem.h>
#include <trackview.h>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMainWindow>
#include <MidiManager.h>
#include <QGraphicsScene>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget *centralWidget;

public:
    explicit MainWindow(MidiManager *mngr,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_quitButton_clicked();

    void openFile();

    void on_actionSave_triggered();

    void on_PauseButton_clicked(int type);

    void on_StartButton_clicked();

    void playSong();

    void updatePROLL(int x,int y,int width,int start, int length);

    void on_actionPlay_triggered();


    void deleteAllNotes();

private:
   // Ui::MainWindow *ui;

    QMenu *fileMenu;
    QMenu *editMenu;
    QAction *saveAction;
    QAction *openFileAction;
    QAction *playSongAction;
    QAction *pauseAction;
    QAction *deleteAllNotesAction;

    void setUpMenuBar();
};

#endif // MAINWINDOW_H
