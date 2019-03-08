#include "browser.h"
#include "folderview.h"
Browser::Browser(FolderViewAbstractModel *qmodel, MasterTrack *master)
{
    mainLayout = new QVBoxLayout;
    tabWidget  = new QTabWidget;
    folderView = new FolderView(qmodel,master);
    tabWidget->addTab(folderView,"Browser");
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}
