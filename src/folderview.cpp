#include "folderview.h"
#include <QDebug>
#include <src/audioengine.h>
#include <src/plugineditorcontainer.h>

int FolderView::tempFolderID = 0;

FolderView::FolderView()
{
    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    setMaximumWidth(300);
    //  setMinimumWidth(150);
    setBaseSize(300,150);
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    setLayout(vLayout);
    QFileSystemModel *model = new QFileSystemModel;

    model->setRootPath(QDir::currentPath());
    model->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    model->setNameFilters(QStringList() << "*.dll");
    model->setNameFilterDisables(false);
    list = new QTreeView();

    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setModel(model);
    list->setRootIndex(model->index(path));


    QPalette pal = palette();
    setAutoFillBackground(true);
    pal.setColor(QPalette::Background, Qt::lightGray);
    setPalette(pal);
    vLayout->addWidget(list);
    QObject::connect(list,&QListView::doubleClicked,this,&FolderView::itemDoubleClicked);

    list->hideColumn(1);
    list->hideColumn(2);
    list->hideColumn(3);
    list->setHeaderHidden(true);
    list->setStyleSheet("QTreeView {background-color: transparent; }");
}



void FolderView::itemDoubleClicked()
{
    QModelIndex index = list->currentIndex();
    QString pluginName = index.data(Qt::DisplayRole).toString();
    QString tempPath = QString(QDir::current().path()+"/TempPlugins/%1.dll").arg(FolderView::tempFolderID++);
    qDebug() << tempPath;
    if (QFile::exists(tempPath))
    {
        QFile::remove(tempPath);
    }

    if (!QFile::copy(path + pluginName, tempPath))
    {
        qDebug() << QDir::current().path();
        qDebug() << "Could not copy plugin";
        return;
    }
    if (isPluginContainerWidget)
    {
        pluginContainer->FolderViewDoubleClicked(pluginName,tempPath);
    }
    else
    {
        pRollContainer->propogateFolderViewDoubleClicked(pluginName,tempPath,path + pluginName);
    }


}

