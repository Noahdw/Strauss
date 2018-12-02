#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <src/pianorollitem.h>
#include <QDebug>
#include <src/mainwindow.h>
#include <random>
#include <src/trackmidiview.h>
#include "src/plugintrackview.h"

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(80,255);

TrackView::TrackView(mTrack *track, TrackMidiView *trackMidiView,TrackContainer *trackContainer, QWidget *parent) : QFrame(parent)
{
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    track_container = trackContainer;
    setStyleSheet("QFrame { background-color: lightGray; border: 1px solid black; }");
    this->track = track;
    track_midi_view = trackMidiView;
    setMinimumWidth(widgetWidth);
    setMaximumWidth(widgetWidth);
    setMinimumHeight(115);
    setMaximumHeight(115);

    instrumentLabel = new QLineEdit(track->instrumentName);
    instrumentLabel->setReadOnly(true);
    instrumentLabel->setMaximumWidth(widgetWidth-10);
    instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
    instrumentLabel->setFrame(false);
    instrumentLabel->installEventFilter(this);
    instrumentLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    if (instrumentLabel->text() == "")
    {
        instrumentLabel->setText("new track");
    }
    muteBox    = new QCheckBox("Mute",this);
    recordBox  = new QCheckBox("Record",this);
    showButton = new QPushButton("Show",this);
    comboBox = new QComboBox;
    showButton->setCheckable(true);

    QVBoxLayout *vlayout = new QVBoxLayout;
    recordBox->setChecked(true);
    vlayout->setContentsMargins(10,5,10,5);
    vlayout->setAlignment(Qt::AlignTop);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addWidget(muteBox);
    vlayout->addWidget(recordBox);
    vlayout->addWidget(showButton);
    showButton->setFixedSize(50,20);
    vlayout->addWidget(comboBox);
    setLayout(vlayout);

    plugin.host = new Vst2HostCallback(track);
    randomRed = distribution(generator);
    randomGreen = distribution(generator);
    randomBlue = distribution(generator);
    connect(instrumentLabel, &QLineEdit::customContextMenuRequested,this,&TrackView::ShowContextMenu);
    QObject::connect(recordBox,&QCheckBox::stateChanged,this,&TrackView::notifyRecordingChange);
    QObject::connect(muteBox,  &QCheckBox::stateChanged,this,&TrackView::notifyMuteChange);
    QObject::connect(showButton,&QPushButton::clicked,this,&TrackView::showPlugin);
    QObject::connect(this, &QWidget::customContextMenuRequested,
                     this,&TrackView::ShowContextMenu);
    connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=](int index){ comboBoxIdChanged(index);}); // wtf
    plugin.host->setCanRecord(true);

    brush = QBrush(QColor(randomRed, randomGreen, randomBlue));
    comboBox->addItem("None");
    comboBox->setMaxCount(24);

}
bool TrackView::eventFilter(QObject *target, QEvent *event)
{
    // qDebug() << event->type();
    if(target == instrumentLabel)
    {
        if (event->type() == QEvent::FocusOut)
        {
            instrumentLabel->setReadOnly(true);
            instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
            canEditLine = false;
            instrumentLabel->clearFocus();
            pluginTrack->setTrackName(instrumentLabel->text());
            return false;
        }
        if (canEditLine)
        {
            auto keyPress = dynamic_cast<QKeyEvent*>(event);
            if (keyPress && ((keyPress->key() == Qt::Key_Enter) || (keyPress->key() == Qt::Key_Return)))
            {
                instrumentLabel->setReadOnly(true);
                instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
                canEditLine = false;
                instrumentLabel->clearFocus();
            }
            return false;
        }
        if (event->type() == QEvent::MouseButtonPress )
        {
            if (canEditLine)
            {
                return false;
            }
            else
            {
                emit trackClickedOn(this);
                return true;
            }
        }

    }

    return false;
    // return TrackView::eventFilter(target,event);
}
void TrackView::addPluginFromPath(QString filepath, QString name, QString actualPath)
{
    if (plugin.effect == NULL)
    {
        qDebug() << "No plugin is currently set";

        QByteArray array = filepath.toLocal8Bit();
        char* file = array.data();

        plugin.effect = plugin.host->loadPlugin(file,name.left(name.size() - 4).toLocal8Bit().data());
        if (plugin.effect == NULL)
        {
            qDebug() << "NULLPTR PLUGIN: in loadPlugin";
            return;
        }
        int state = plugin.host->configurePluginCallbacks(plugin.effect);
        if (state == -1)
        {
            qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
            return;
        }
        instrumentLabel->setText(name.left(name.size() - 4));
        plugin.host->startPlugin(plugin.effect);
        MainWindow::pluginHolderVec.append(&plugin);
        pluginTrack->addPlugin(&plugin);
        plugin.host->isMasterPlugin = true;
        plugin.host->masterPluginTrackView = this->pluginTrack;
        plugin.host->actual_url = actualPath;

        for (int i = 0; i <  plugin.host->numParams(plugin.effect); ++i)
        {
            QString name = plugin.host->getParamName(plugin.effect,i);
            comboBox->addItem(name);
        }
    }

}

void TrackView::addPluginFromLoadProject(QString filepath)
{
    if (filepath == "")
    {
        return;
    }
    QString tempPath = QString(QDir::current().path()+"/TempPlugins/%1.dll").arg(FolderView::tempFolderID++);
    if (QFile::exists(tempPath))
    {
        QFile::remove(tempPath);
    }

    if (!QFile::copy(filepath, tempPath))
    {
        qDebug() << QDir::current().path();
        qDebug() << "Could not copy plugin";
        return;
    }
    int last = filepath.lastIndexOf("/") + 1;
    QString name = filepath.right(filepath.length() - last);
    addPluginFromPath(tempPath,name,filepath);
}


// Will delete this class. Do not call any member variables/functions after container call.
void TrackView::deleteTrack()
{
    pluginTrack->deleteLater();
    plugin.host->markForDeletion();
    //track_midi_view->deleteLater();
    //delete this;
    track_container->deleteTrack(this,track_midi_view);
}

QString TrackView::getTrackName() const
{
    return instrumentLabel->text();
}

TrackMidiView *TrackView::getTrackMidiView()
{
    return track_midi_view;
}

void TrackView::notifyMuteChange(int state)
{
    if(state)
    {
        plugin.host->isMuted = true;
    }
    else
    {
        plugin.host->isMuted = false;
    }
}

void TrackView::notifyRecordingChange(int state)
{
    if (state)
    {
        plugin.host->setCanRecord(true);
    }
    else
    {
        plugin.host->setCanRecord(false);
    }
}

void TrackView::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(("Context menu"), this);
    QAction renameAction("Rename", this);
    QAction deleteAction("Delete track",this);
    connect(&renameAction,&QAction::triggered,this,&TrackView::renameTrack);
    connect(&deleteAction,&QAction::triggered,this,&TrackView::deleteTrack);

    contextMenu.addAction(&renameAction);
    contextMenu.addAction(&deleteAction);
    contextMenu.exec(mapToGlobal(pos));
}

void TrackView::showPlugin()
{
    if (plugin.host)
    {
        plugin.host->showPlugin();
    }
}

void TrackView::renameTrack()
{
    instrumentLabel->setReadOnly(false);
    canEditLine = true;
    instrumentLabel->setFocus();
    instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(randomRed, 50, 50, 255); }");//?? works though
}

void TrackView::comboBoxIdChanged(int index)
{

}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(brush);
    painter.drawRect(0,0,width() - 1,instrumentLabel->height() + 5);
}

void TrackView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        ShowContextMenu(event->pos());
    }


    emit trackClickedOn(this);
    QFrame::mousePressEvent(event);
}





