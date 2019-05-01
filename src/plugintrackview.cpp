#include "plugintrackview.h"
#include "src/trackwidget.h"
#include "src/pluginview.h"
#include "src/plugineditorcontainer.h"
#include "trackmidi.h"
/*
This is the representation of a track that is shown on the
pluginEditorContainer view. It is simillar to the track shown on
a PianoRoll view but it contains much more details and options
*/
PluginTrackView::PluginTrackView(TrackWidget * track)
{
    trackView = track;
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
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
    panSlider = new QSlider;
    QLabel *panLabel = new QLabel("Pan");

    volumeSlider->setRange(0,300);
    volumeSlider->setFixedHeight(15);
    volumeSlider->setOrientation(Qt::Horizontal);
    volumeSlider->setValue(200);

    panSlider->setRange(-100,100);
    panSlider->setFixedHeight(15);
    panSlider->setOrientation(Qt::Horizontal);
    panSlider->setValue(0);


    recordBox->setChecked(false);
    vlayout->setContentsMargins(10,0,10,10);
    vlayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addSpacing(5);
    vlayout->addWidget(muteBox);
    vlayout->addWidget(recordBox);
    vlayout->addWidget(volumeLabel);
    vlayout->addWidget(volumeSlider);
    vlayout->addWidget(panLabel);
    vlayout->addWidget(panSlider);
    setLayout(vlayout);

    setFrameShape(QFrame::Box);
    red   = track->randomRed;
    green = track->randomGreen;
    blue  = track->randomBlue;
    brush = QBrush(Qt::lightGray);
    pen   = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    brush.setColor(QColor(red, green, blue));
   // setStyleSheet("QFrame { background-color: lightGray; border: 1px solid black; border-radius: 4px; }");



    connect(volumeSlider,&QSlider::sliderReleased,this,&PluginTrackView::volumeChanged);
    connect(panSlider,&QSlider::sliderReleased,this,&PluginTrackView::panChanged);
    midiTrack = track->midiTrack();

}

PluginTrackView::~PluginTrackView()
{
    for (std::vector< PluginView* >::iterator it = plugins.begin() ; it != plugins.end(); ++it)
    {
        delete (*it);
    }
    pluginEditorContainer->lastPluginTrack = NULL;
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
//    pluginEditorContainer->masterTrack->setCurrentTrack(midiTrack);
//    pluginEditorContainer->switchPluginViews(this);
//    pluginEditorContainer->pluginTrackClickedOn();
//    pluginEditorContainer->masterTrack->setCurrentTrack(midiTrack);
//    clickedOn(true);
    QFrame::mousePressEvent(event);
}

void PluginTrackView::mouseDoubleClickEvent(QMouseEvent *event)
{
    pluginEditorContainer->switchPluginViews(this);
    pluginEditorContainer->pluginTrackClickedOn();
    clickedOn(true);
    QFrame::mouseDoubleClickEvent(event);
}

void PluginTrackView::addPlugin(Vst2AudioPlugin * vst2Plugin)
{
    if (pluginEditorContainer->lastPluginTrack == NULL)
    {
        pluginEditorContainer->lastPluginTrack = this;
    }

    PluginView * pluginView = new PluginView(vst2Plugin);
    pluginView->hide();
    plugins.push_back(pluginView);
    pluginEditorContainer->switchPluginViews(this);
}

void PluginTrackView::clickedOn(bool state)
{
    if (state)
    {
        pen.setStyle(Qt::DashLine);
        setProperty("clicked",true);
        style()->unpolish(this);
        style()->polish(this);
    }
    else
    {
        pen.setStyle(Qt::SolidLine);
        setProperty("clicked",false);
        style()->unpolish(this);
        style()->polish(this);
    }
    update();
}

void PluginTrackView::setTrackName(QString name)
{
    instrumentLabel->setText(name);
}

void PluginTrackView::panChanged()
{
    float p = panSlider->value();
   // midiTrack->masterPlugin().pan = p / 200.0;
}

void PluginTrackView::volumeChanged()
{
    double volume = (double)volumeSlider->value() / 100.0;
   // midiTrack->masterPlugin().trackVolume = volume;
}
