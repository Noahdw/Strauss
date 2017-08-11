#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <midimanager.h>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <midiplayer.h>
#include<QtConcurrent/QtConcurrent>
#include <pianoroll.h>

MidiPlayer player;
MidiManager *manager;

MainWindow::MainWindow(MidiManager *mngr,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    manager = mngr;
    ui->setupUi(this);
   // pRoll.createPianoRoll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitButton_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
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


             //QFuture<void> future = QtConcurrent::run(&player,&MidiPlayer::playMidiFile,manager);
            //manager->printMidiToScreen();

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
{}
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
