#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <pianorollitem.h>
#include <QDebug>
#include <mainwindow.h>
TrackView::TrackView(mTrack *track,QWidget *parent) : QFrame(parent)
{

    this->track = track;
    instrumentName = track->instrumentName;
    if (instrumentName == "") {
        instrumentName = "new track";
    }
    instrumentLabel = new QLabel(instrumentName);
    instrumentLabel->setMaximumWidth(widgetWidth-10);

    vlayout = new QVBoxLayout;
    vlayout->addWidget(instrumentLabel);
    setLayout(vlayout);
    setFixedSize(widgetWidth,70);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(0);
    setMidLineWidth(0);
    plugin.host = new Vst2HostCallback(track);

}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);

    QBrush brush(Qt::lightGray);
    painter->setBrush(brush);

    painter->drawRect(0,0,widgetWidth,70);

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
         }
         MainWindow::pluginHolderVec.append(&plugin);
    }
    else
    {

    }
    qDebug() << id;
}





