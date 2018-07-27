#include "folderview.h"
#include <QDebug>
#include <src/audiomanager.h>
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
    list = new QListView();
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setModel(model);
    list->setRootIndex(model->index(path));
    list->setStyleSheet("background-color: transparent;");
    QPalette pal = palette();
    setAutoFillBackground(true);
    pal.setColor(QPalette::Background, Qt::lightGray);
    setPalette(pal);
    vLayout->addWidget(list);
    QObject::connect(list,&QListView::doubleClicked,this,&FolderView::itemDoubleClicked);
}

void FolderView::itemDoubleClicked()
{
    QModelIndex index = list->currentIndex();
    QString pluginName = index.data(Qt::DisplayRole).toString();
    pRollContainer->propogateFolderViewDoubleClicked(pluginName,path);
}
