#include "pluginview.h"
#include "src/vst2hostcallback.h"

PluginView::PluginView(Vst2HostCallback *vst2Plugin)
{
    setMaximumSize(150,150);
    setMinimumSize(150,150);
    vLayout = new QVBoxLayout;
    pluginNameLabel = new QLabel;
    showButton    = new QPushButton("Show",this);
    disableButton = new QPushButton("Disable",this);
    setLayout(vLayout);

    vLayout->setAlignment(Qt::AlignTop);
    plugin = vst2Plugin;
    pluginName.sprintf("%s",plugin->pluginName);
    pluginNameLabel->setText(pluginName);
    vLayout->addWidget(pluginNameLabel);
    vLayout->addWidget(showButton);
    vLayout->addWidget(disableButton);
    setFrameShape(QFrame::Box);
    disableButton->setCheckable(true);

    QObject::connect(showButton,&QPushButton::clicked,this,&PluginView::showPlugin);
    QObject::connect(disableButton,&QPushButton::toggled,this,&PluginView::disablePlugin);
}

void PluginView::showPlugin()
{
    plugin->showPlugin();
}

void PluginView::disablePlugin(bool state)
{
    if (plugin->isMasterPlugin)
    {
        plugin->isMuted = state;
    }
    else
    {
        plugin->canPlay = !state;
    }

}
