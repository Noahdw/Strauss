#include "plugineditorcontainer.h"
#include "src/trackwidget.h"
#include "src/plugintrackview.h"
#include "src/pluginview.h"

/*
This class represents the base screen that will be shown when user presses ALT.
It shows all the tracks and their corresponding plugins.
It is simillar to the screen shown when you press TAB in Abelton.
*/

PluginEditorContainer::PluginEditorContainer(FolderViewAbstractModel *model,MasterTrack *masterTrack)
{
    this->masterTrack = masterTrack;
    vLayout         = new QVBoxLayout;
    hlayout         = new QHBoxLayout;
    hTopLayout      = new QHBoxLayout;
    hBotttomLayout  = new QHBoxLayout;
    trackScrollArea = new QScrollArea;
    folderView      = new FolderView(model,masterTrack);

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
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    QFrame * widget = new QFrame;

    widget->setLayout(hTopLayout);
    trackScrollArea->setWidget(widget);

    widget->show();
    trackScrollArea->show();
}

PluginTrackView* PluginEditorContainer::addTrack(TrackWidget *track)
{
    PluginTrackView * pluginTrack = new PluginTrackView(track);
    pluginTrack->pluginEditorContainer = this;
    hTopLayout->addWidget(pluginTrack);
    switchPluginViews(pluginTrack);
    return pluginTrack;
}



void PluginEditorContainer::switchPluginViews(PluginTrackView *pluginTrack)
{
    if (!lastPluginTrack)
    {
        return;
    }
    for (uint i = 0; i < lastPluginTrack->plugins.size(); ++i)
    {
        hBotttomLayout->removeWidget((lastPluginTrack->plugins.at(i)));
        lastPluginTrack->plugins.at(i)->hide();
    }

    for (uint i = 0; i < pluginTrack->plugins.size(); ++i)
    {
        hBotttomLayout->addWidget(pluginTrack->plugins.at(i));
        pluginTrack->plugins.at(i)->show();
    }
    lastPluginTrack = pluginTrack;
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
