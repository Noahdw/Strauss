#include "pluginview.h"
#include "src/vst2hostcallback.h"

PluginView::PluginView(pluginHolder *pholder)
{
    setMaximumSize(150,150);
    setMinimumSize(150,150);
    vLayout = new QVBoxLayout;
    pluginNameLabel = new QLabel;
    showButton    = new QPushButton("Show",this);
    disableButton = new QPushButton("Disable",this);
    setLayout(vLayout);

    vLayout->setAlignment(Qt::AlignTop);
    holder = pholder;
    pluginName.sprintf("%s",pholder->host->pluginName);
    pluginNameLabel->setText(pluginName);
    vLayout->addWidget(pluginNameLabel);
    vLayout->addWidget(showButton);
    vLayout->addWidget(disableButton);
    setFrameShape(QFrame::Box);
    disableButton->setCheckable(true);

    QObject::connect(showButton,&QPushButton::clicked,this,&PluginView::showPlugin);
    QObject::connect(disableButton,&QPushButton::toggled,this,&PluginView::disablePlugin);
    setStyleSheet("QFrame { background-color: lightGray; border: 1px solid black; border-radius: 4px; }");
}

void PluginView::showPlugin()
{
    holder->host->showPlugin();
}

void PluginView::paintEvent(QPaintEvent *event)
{
   // QPainter painter(this);
  //  painter.setBrush(QBrush(Qt::lightGray));
  //  painter.drawRect(0,0,width() - 1,height() - 1);
}

void PluginView::disablePlugin(bool state)
{
    if (holder->host->isMasterPlugin)
    {
        holder->host->isMuted = state;
    }
    else
    {
        holder->host->canPlay = !state;
    }

}
