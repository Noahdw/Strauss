#include "plugineditorcontainer.h"
#include "src/trackview.h"
#include "src/plugintrackview.h"
#include "src/pluginview.h"

/*
This class represents the base screen that will be shown when user presses ALT.
It shows all the tracks and their corresponding plugins.
It is simillar to the screen shown when you press TAB in Abelton.
*/

PluginEditorContainer::PluginEditorContainer(FolderViewAbstractModel *model)
{
    vLayout         = new QVBoxLayout;
    hlayout         = new QHBoxLayout;
    hTopLayout      = new QHBoxLayout;
    hBotttomLayout  = new QHBoxLayout;
    trackScrollArea = new QScrollArea;
    folderView      = new FolderView(model);
    vLayout->addWidget(trackScrollArea);
    vLayout->addLayout(hBotttomLayout);
    hlayout->addWidget(folderView);
    hlayout->addLayout(vLayout);
    setLayout(hlayout);

    vLayout->setAlignment(Qt::AlignTop);
    hTopLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    hTopLayout->setSpacing(1);
    hTopLayout->setContentsMargins(0,0,0,0);
    hTopLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    hBotttomLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    folderView->isPluginContainerWidget = true;
    folderView->pluginContainer = this;
    trackScrollArea->setWidgetResizable(true);
   // trackScrollArea->setFixedSize(1000,300);
   // trackScrollArea->setMinimumHeight(300);
    //trackScrollArea->setMaximumHeight(300);
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    QWidget * widget = new QWidget;

    widget->setLayout(hTopLayout);
    trackScrollArea->setWidget(widget);
   // trackScrollArea->setLayout(hTopLayout);
    widget->show();
    trackScrollArea->show();
}

PluginTrackView* PluginEditorContainer::addTrack(TrackView *track)
{
    PluginTrackView * pluginTrack = new PluginTrackView(track);
    pluginTrack->plugin_editor_container = this;
    hTopLayout->addWidget(pluginTrack);
    switchPluginViews(pluginTrack);
    return pluginTrack;
}

void PluginEditorContainer::addPlugin(PluginTrackView *pluginTrack, Vst2HostCallback *vst2)
{
    // PluginView * plugin = new PluginView(vst2);
    // pluginTrack->addPlugin(plugin);
    // switchPluginViews(pluginTrack);
}

void PluginEditorContainer::switchPluginViews(PluginTrackView *pluginTrack)
{
    if (!lastPluginTrack)
    {
        return;
    }
    for (int i = 0; i < lastPluginTrack->plugins.size(); ++i)
    {
        hBotttomLayout->removeWidget((lastPluginTrack->plugins.at(i)));
        lastPluginTrack->plugins.at(i)->hide();
    }

    for (int i = 0; i < pluginTrack->plugins.size(); ++i)
    {
        hBotttomLayout->addWidget(pluginTrack->plugins.at(i));
        pluginTrack->plugins.at(i)->show();
    }
    lastPluginTrack = pluginTrack;
}

void PluginEditorContainer::FolderViewDoubleClicked(QString name, QString filepath)
{
    if (lastPluginTrack == NULL)
    {
        return;
    }
    pluginHolder *plugin = new pluginHolder;
    Vst2HostCallback * vst2 = new Vst2HostCallback;

    plugin->host = vst2;

    QByteArray array = filepath.toLocal8Bit();
    char* file = array.data();
    plugin->effect = plugin->host->loadPlugin(file,name.left(name.size() - 4).toLocal8Bit().data());
    if (plugin->effect == NULL) {
        qDebug() << "NULLPTR PLUGIN: in loadPlugin";
        return;
    }
    int state = plugin->host->configurePluginCallbacks(plugin->effect);
    if (state == -1) {
        qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
        return;
    }

    plugin->host->startPlugin(plugin->effect);
    lastPluginTrack->addPlugin(plugin);
}

void PluginEditorContainer::pluginTrackClickedOn()
{
    for (int i = 0; i < hTopLayout->count(); ++i) {
        auto pluginTrack = dynamic_cast<PluginTrackView*>(hTopLayout->itemAt(i)->widget());

        if(pluginTrack)
        {
            pluginTrack->clickedOn(false);
        }
    }
}
