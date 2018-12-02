#include "plugintrackview.h"
#include "src/trackview.h"
#include "src/pluginview.h"
#include "src/plugineditorcontainer.h"
/*
This is the representation of a track that is shown on the
pluginEditorContainer view. It is simillar to the track shown on
a PianoRoll view but it contains much more details and options
*/
PluginTrackView::PluginTrackView(TrackView * track)
{
    trackView = track;

    //setFixedSize(90,90);
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
   // setMinimumSize(120,100);
    setMaximumSize(100,200);
    instrumentLabel = new QLineEdit(track->getTrackName());
    instrumentLabel->setReadOnly(true);
    instrumentLabel->setMaximumWidth(width()-10);
    instrumentLabel->setStyleSheet("QLineEdit { background-color: rgba(0, 0, 0, 0); }");
    instrumentLabel->setFrame(false);

    muteBox    = new QCheckBox("Mute",this);
    recordBox  = new QCheckBox("Record",this);
    vlayout    = new QVBoxLayout;
    volumeSlider = new QSlider;
    QLabel *volumeLabel = new QLabel("Volume");

    volumeSlider->setRange(0,300);
    volumeSlider->setFixedHeight(15);
    volumeSlider->setOrientation(Qt::Horizontal);
    volumeSlider->setValue(100);

    recordBox->setChecked(false);
   // vlayout->setMargin(0);
    vlayout->setContentsMargins(10,0,10,10);
    vlayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addSpacing(5);
    vlayout->addWidget(muteBox);
    vlayout->addWidget(recordBox);
    vlayout->addWidget(volumeLabel);
    vlayout->addWidget(volumeSlider);
  //  vlayout->addSpacerItem(new QSpacerItem(0,100));
    setLayout(vlayout);

    setFrameShape(QFrame::Box);
    red   = track->randomRed;
    green = track->randomGreen;
    blue  = track->randomBlue;
    brush = QBrush(Qt::lightGray);
    pen   = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    brush.setColor(QColor(red, green, blue));
    setStyleSheet("QFrame { background-color: lightGray; border: 1px solid black; border-radius: 4px; }");



    connect(volumeSlider,&QSlider::sliderReleased,this,&PluginTrackView::volumeChanged);
}

PluginTrackView::~PluginTrackView()
{
    for (std::vector< PluginView* >::iterator it = plugins.begin() ; it != plugins.end(); ++it)
    {
        delete (*it);
    }
    plugin_editor_container->lastPluginTrack = NULL;
}

void PluginTrackView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(0,0,width() - 1, instrumentLabel->height() + 5);
    QFrame::paintEvent(event);
}

void PluginTrackView::mousePressEvent(QMouseEvent *event)
{
    plugin_editor_container->switchPluginViews(this);
    plugin_editor_container->pluginTrackClickedOn();
    clickedOn(true);
    QFrame::mousePressEvent(event);
}

void PluginTrackView::mouseDoubleClickEvent(QMouseEvent *event)
{
    plugin_editor_container->switchPluginViews(this);
    plugin_editor_container->pluginTrackClickedOn();
    clickedOn(true);
    QFrame::mouseDoubleClickEvent(event);
}

void PluginTrackView::addPlugin(pluginHolder *holder)
{
    if (plugin_editor_container->lastPluginTrack == NULL)
    {
        plugin_editor_container->lastPluginTrack = this;
    }

    PluginView * plugin = new PluginView(holder);
    plugin->hide();
    plugins.push_back(plugin);
    plugin_editor_container->switchPluginViews(this);
}

void PluginTrackView::clickedOn(bool state)
{
    if (state)
    {
        pen.setStyle(Qt::DashLine);
        // QString style = QString("QFrame { background-color:  rgb(%1,%2,%3); border: 0px ; }").arg(red).arg(green).arg(blue);
        setStyleSheet("QFrame { background-color: rgb(170,170,170); border: 1px solid black; border-radius: 4px; }");
    }
    else
    {
        pen.setStyle(Qt::SolidLine);
        setStyleSheet("QFrame { background-color: lightGray; border: 1px solid black; border-radius: 4px; }");
    }
    update();
}

void PluginTrackView::setTrackName(QString name)
{
    instrumentLabel->setText(name);
}

void PluginTrackView::volumeChanged()
{
    double volume = (double)volumeSlider->value() / 100.0;
    trackView->plugin.host->trackVolume = volume;
}
