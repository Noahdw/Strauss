#include "plugintrackview.h"
#include "src/trackview.h"
#include "src/pluginview.h"
#include "src/plugineditorcontainer.h"

PluginTrackView::PluginTrackView(TrackView * track)
{
    trackView = track;
    setMaximumHeight(90);
    setMinimumHeight(90);
    setMaximumWidth(90);
    instrumentName = track->instrumentName;

    instrumentLabel = new QLineEdit(instrumentName);
    instrumentLabel->setReadOnly(true);
    instrumentLabel->setMaximumWidth(width()-10);
    instrumentLabel->setStyleSheet("* { background-color: rgba(0, 0, 0, 0); }");
    instrumentLabel->setFrame(false);

    muteBox    = new QCheckBox("Mute",this);
    recordBox  = new QCheckBox("Record",this);

    vlayout    = new QVBoxLayout;
    recordBox->setChecked(true);

    vlayout->setAlignment(Qt::AlignTop);
    vlayout->addWidget(instrumentLabel,0,Qt::AlignTop|Qt::AlignLeft);
    vlayout->addSpacing(5);
    vlayout->addWidget(muteBox);
    vlayout->addWidget(recordBox);

    setLayout(vlayout);

    setFrameShape(QFrame::Box);
    //setMidLineWidth(0);
    red = track->randomRed;
    green = track->randomGreen;
    blue = track->randomBlue;

}

void PluginTrackView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(Qt::lightGray);
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height()-1);
    brush.setColor((QColor(red, green, blue)));
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height()/3);
}

void PluginTrackView::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);
}

void PluginTrackView::mouseDoubleClickEvent(QMouseEvent *event)
{
     container->switchPluginViews(this);
     QFrame::mouseDoubleClickEvent(event);
}

void PluginTrackView::addPlugin(pluginHolder *holder)
{
    if (container->lastPluginTrack == NULL)
    {
          container->lastPluginTrack = this;
    }

    PluginView * plugin = new PluginView(holder);
    plugin->hide();
    plugins.push_back(plugin);
    container->switchPluginViews(this);
}
