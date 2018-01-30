#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <src/pianorollitem.h>
#include <QDebug>
#include <src/mainwindow.h>
#include <random>
#include <src/trackmidiview.h>

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1,255);

TrackView::TrackView(mTrack *track,QWidget *parent) : QFrame(parent)
{

    this->track = track;
    instrumentName = track->instrumentName;
    if (instrumentName == "") {
        instrumentName = "new track";
    }
    instrumentLabel = new QLabel(instrumentName);
    instrumentLabel->setMaximumWidth(widgetWidth-10);
    muteBox = new QCheckBox("Mute",this);

    vlayout = new QVBoxLayout;
    vlayout->setAlignment(Qt::AlignTop);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addSpacing(5);
    vlayout->addWidget(muteBox);
    setLayout(vlayout);
    // setLineWidth(0);

    setFixedSize(widgetWidth+lineWidth()*2,70);
   // setFrameStyle(QFrame::Box | QFrame::Plain);
    setFrameShape(QFrame::Box);
    //setMidLineWidth(0);
    plugin.host = new Vst2HostCallback(track);
    randomRed = distribution(generator);
    randomGreen = distribution(generator);
    randomBlue = distribution(generator);

    QObject::connect(muteBox,&QCheckBox::stateChanged,this,&TrackView::notifyMuteChange);

}

void TrackView::folderViewItemDoubleClicked(QString filepath, QString name)
{
    if (plugin.effect ==NULL) {
         qDebug() << "No plugin is currently set";

             QByteArray array = filepath.toLocal8Bit();
             char* file = array.data();

             plugin.effect = plugin.host->loadPlugin(file);
             if (plugin.effect == NULL) {
                 qDebug() << "NULLPTR PLUGIN: in loadPlugin";
                 return;
             }
             int state = plugin.host->configurePluginCallbacks(plugin.effect);
             if (state == -1) {
                 qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
                 return;
             }
             instrumentLabel->setText(name);
             plugin.host->startPlugin(plugin.effect);
             MainWindow::pluginHolderVec.append(&plugin);
    }

}

void TrackView::notifyMuteChange(int state)
{
    if(state){
       plugin.host->isMuted = true;
    }
    else{
         plugin.host->isMuted = false;
    }
}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);

    QBrush brush(Qt::lightGray);
    painter->setBrush(brush);
    painter->drawRect(0,0,widgetWidth,70-1);
    brush.setColor((QColor(randomRed, randomGreen, randomBlue)));
    painter->setBrush(brush);
    painter->drawRect(0,0,widgetWidth,30);

}

void TrackView::mousePressEvent(QMouseEvent *event)
{
    //
   emit trackClickedOn(id);
    if (plugin.effect ==NULL) {
         qDebug() << "No plugin is currently set";

         QFileDialog dialog;
         QString fileName  = dialog.getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("dll Files (*.dll)"));

         qDebug() << fileName;
         if (!fileName.isEmpty()) {
             QByteArray array = fileName.toLocal8Bit();
             char* file = array.data();

             plugin.effect = plugin.host->loadPlugin(file);
             if (plugin.effect == NULL) {
                 qDebug() << "NULLPTR PLUGIN: in loadPlugin";
                 return;
             }
             int state = plugin.host->configurePluginCallbacks(plugin.effect);
             if (state == -1) {
                 qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
                 return;
             }
             plugin.host->startPlugin(plugin.effect);
             MainWindow::pluginHolderVec.append(&plugin);
         }

    }
    else
    {

    }
    qDebug() << id;
}





