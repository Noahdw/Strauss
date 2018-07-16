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
    setMinimumHeight(70);
    setMaximumHeight(90);
    this->track = track;
    instrumentName = track->instrumentName;
    if (instrumentName == "") {
        instrumentName = "new track";
    }
    instrumentLabel = new QLineEdit(instrumentName);
    instrumentLabel->setReadOnly(true);
    instrumentLabel->setMaximumWidth(widgetWidth-10);
    instrumentLabel->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
    instrumentLabel->setFrame(false);
    instrumentLabel->installEventFilter(this);
    // instrumentLabel->setAttribute(Qt::Wa_ws,0);
    muteBox   = new QCheckBox("Mute",this);
    recordBox = new QCheckBox("Record",this);
    recordBox->setChecked(true);
    vlayout   = new QVBoxLayout;
    vlayout->setAlignment(Qt::AlignTop);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addSpacing(5);
    vlayout->addWidget(muteBox);
    vlayout->addWidget(recordBox);
    setLayout(vlayout);
    // setLineWidth(0);

    // setFixedSize(widgetWidth+lineWidth()*2,90);
    // setFrameStyle(QFrame::Box | QFrame::Plain);
    setFrameShape(QFrame::Box);
    //setMidLineWidth(0);
    plugin.host = new Vst2HostCallback(track);
    randomRed = distribution(generator);
    randomGreen = distribution(generator);
    randomBlue = distribution(generator);

    QObject::connect(recordBox,&QCheckBox::stateChanged,this,&TrackView::notifyRecordingChange);
    QObject::connect(muteBox,  &QCheckBox::stateChanged,this,&TrackView::notifyMuteChange);
    QObject::connect(this, &QWidget::customContextMenuRequested,
                     this,&TrackView::ShowContextMenu);

}
bool TrackView::eventFilter(QObject *target, QEvent *event)
{
    //idk how to do this
    qDebug() << event->type();
    if(target == instrumentLabel )
    {
        if (canEditLine)
        {
            return false;
        }else
        {

        }
        if (event->type() == QEvent::MouseButtonPress )
        {
            if (canEditLine)
            {
                return false;
            }
            else
            {
                // event->ignore();
                emit trackClickedOn(id);
                return true;
            }
        }
        if (!canEditLine && (event->type() == QEvent::ToolTip || event->type() == QEvent::MouseButtonPress ))
        {
            //return true;
        }

    }

    return false;
    // return TrackView::eventFilter(target,event);
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

void TrackView::notifyRecordingChange(int state)
{
    if (state)
    {
        plugin.host->setCanRecord(true);
    }else
    {
        plugin.host->setCanRecord(false);
    }
}

void TrackView::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(("Context menu"), this);
    QAction renameAction("Rename", this);

    connect(&renameAction,&QAction::triggered,this,&TrackView::renameTrack);

    contextMenu.addAction(&renameAction);
    contextMenu.exec(mapToGlobal(pos));
}

void TrackView::renameTrack()
{
    instrumentLabel->setReadOnly(false);
    canEditLine = true;
    instrumentLabel->setStyleSheet("* { background-color: rgba(randomRed, 50, 50, 255); }");//?? works though
}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(Qt::lightGray);
    painter.setBrush(brush);
    painter.drawRect(0,0,widgetWidth,height()-1);
    brush.setColor((QColor(randomRed, randomGreen, randomBlue)));
    painter.setBrush(brush);
    painter.drawRect(0,0,widgetWidth,height()/3);
}

void TrackView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        ShowContextMenu(event->pos());
    }
    else
    {
        instrumentLabel->setReadOnly(true);
        instrumentLabel->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
        canEditLine = false;
    }

    emit trackClickedOn(id);
    QFrame::mousePressEvent(event);
}





