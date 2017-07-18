#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <midimanager.h>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <midi.h>
#include <midiplayer.h>
#include<QtConcurrent/QtConcurrent>
#include <pianoroll.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
   // pRoll.createPianoRoll();
}
  MidiPlayer player;
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
        MidiManager * manager = new MidiManager;

        QString s;

        QByteArray array = manager->ReadMidi(file);
        for (int var = 0; var < array.length(); ++var) {
            qint8 val= array.at(var);
            s.append(QString::number(val) + " ");
        }






        manager->song = manager->Deserialize(array);


             QFuture<void> future = QtConcurrent::run(&player,&MidiPlayer::playMidiFile,manager);





       // player.pausePlayBack();
       // ui->textEdit->setText(s);

        file.close();
    }
}



void MainWindow::on_PauseButton_clicked()
{
    player.pausePlayBack();
}

void MainWindow::on_StartButton_clicked()
{
   player.resumePlayBack();
}

void MainWindow::on_actionSave_triggered()
{}
