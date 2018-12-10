#include "folderview.h"
#include <QDebug>
#include <src/audioengine.h>
#include <src/plugineditorcontainer.h>
#include "src/trackmidi.h"
int FolderView::tempFolderID = 0;

FolderView::FolderView(FolderViewAbstractModel *qmodel, MasterTrack * master)
{
    masterTrack = master;
    model = qmodel;
    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    setMaximumWidth(300);
    setBaseSize(300,150);
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    setLayout(vLayout);
    list = new QTreeView();

    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setModel(model);

    vLayout->addWidget(list);
    QObject::connect(list,&QListView::doubleClicked,this,&FolderView::itemDoubleClicked);

    list->hideColumn(1);
    list->hideColumn(2);
    list->hideColumn(3);
    list->setHeaderHidden(true);

}



void FolderView::itemDoubleClicked()
{
    QModelIndex index = list->currentIndex();
    QString pluginName = index.data(0).toString();
    QString absPath = index.data(0x0100).toString();
//    qDebug() << index.data(1);
//    QString tempPath = QString(QDir::current().path()+"/TempPlugins/%1.dll").arg(FolderView::tempFolderID++);
//    qDebug() << tempPath;
//    if (QFile::exists(tempPath))
//    {
//        QFile::remove(tempPath);
//    }

//    if (!QFile::copy(absPath, tempPath))
//    {
//        qDebug() << absPath;
//        qDebug() << "Could not copy plugin";
//        return;
//    }

    masterTrack->currentTrack()->loadPlugin(absPath);

}

