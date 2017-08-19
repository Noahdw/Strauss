#include "mainwindow.h"
#include "ui_mainwindow.h"
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
MidiPlayer player;
MidiManager *manager;
PianoRoll *pianoRollView;

MainWindow::MainWindow(MidiManager *mngr,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     pianoRollView = new PianoRoll;
     pianoRollView->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    manager = mngr;
    scene = new QGraphicsScene;
    scene->setSceneRect(*pianoRollView->sceneRect);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->setupUi(this);
    setCentralWidget(pianoRollView);

    QObject::connect(pianoRollView,&PianoRoll::addNoteToPROLL,this,&MainWindow::updatePROLL);
    QObject::connect(pianoRollView,&PianoRoll::deleteNotesFromPROLL,this,&MainWindow::deleteFromPROLL);
     QObject::connect(pianoRollView,&PianoRoll::changeSceneRect,this,&MainWindow::updateSceneRect);
    pianoRollView->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePROLL(int x,int y, int width,int start, int length)
{
    PianoRollItem *pNote = new PianoRollItem;
    scene->addItem(pNote);
    pNote->setPos(x,y);
    pNote->setBoundingRect(width);
    manager->updateMidi(127 - y/pNote->keyHeight,80,start,length);
}
void MainWindow::deleteFromPROLL(QGraphicsItem *item)
{

    scene->removeItem(item);
    delete item;
}

void MainWindow::updateSceneRect(QRectF newRect,const QRectF *oldRect,QRectF visibleRect)
{
    scene->setSceneRect(newRect);
    int oldRange = (oldRect->width() - 0);
    int newRange = (newRect.width() - 0);
    foreach (auto item, scene->items(newRect,Qt::IntersectsItemBoundingRect)) {
        PianoRollItem * pNote = dynamic_cast<PianoRollItem*>(item);

        int newX = (((item->x() - 0) * newRange) / oldRange) + 0;
        int newWidth = (((item->boundingRect().width() - 0) * newRange) / oldRange) + 0;
        pNote->setBoundingRect(newWidth);
        pNote->setX(newX);

    }
}

void MainWindow::on_quitButton_clicked()
{
    midiStreamStop(player.outHandle);

    QCoreApplication::quit();
}
//Opens and deserializes a song
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

        //Move later

        int dw = 0;

        for (int i = 1; i < manager->noteVec.length(); i+=3){
            dw += manager->noteVec.at(i);

        }
        int tqn = dw/manager->noteVec.at(0);//tpqn is 0 pos for now, tqn tells total quarter notes



        int curNote = 0;
        int elapsedDW = 0;
        int noteEnd = 0;
        for(int i = 1; i < manager->noteVec.length(); i+=3){


            elapsedDW += manager->noteVec.at(i);
            //indicates a note. ignore other junk for now
            if((manager->noteVec.at(i+2)& 0xF0) ==0x90){
                curNote = (manager->noteVec.at(i+2) >> 8) & 127;
                //now I need to find its note off
                for(int j = i+5; j< manager->noteVec.length(); j+=3){
                    noteEnd+= manager->noteVec.at(j -2);
                    if(((manager->noteVec.at(j) >> 8) & 127) == curNote ){

                        PianoRollItem *pNote = new PianoRollItem;
                        scene->addItem(pNote);
                      pNote->setPos(elapsedDW/tqn*pianoRollView->colSpacing,127*PianoRollItem::keyHeight - curNote*PianoRollItem::keyHeight);

                        qDebug() << elapsedDW/(tqn*pianoRollView->colSpacing);
                      pNote->setBoundingRect(noteEnd/tqn*pianoRollView->colSpacing);
                        noteEnd = 0;
                        break;
                    }
                }

            }
        }
           pianoRollView->notifyViewChanged(manager->noteVec.at(0),tqn);
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
