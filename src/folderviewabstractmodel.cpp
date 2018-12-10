#include "folderviewabstractmodel.h"

/*
I use an QAbstractItemModel because the library QFileSystemModel class only allows a single root path to define
where it should look for files/dirs. The user should be able to supply multiple root paths, however. No to mention
there is benefit in being able to further define the folder filtering to be used so that I don't display empty folders.

*/

FolderViewAbstractModel::FolderViewAbstractModel(const QList<QString> data, QObject *parent)
{
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    auto  rootItem = new TreeItem(rootData);
    setupModelData(rootItem,data);
}
//Update the model with a new list of paths
void FolderViewAbstractModel::setModel(QList<QString> list)
{
    beginResetModel();
    delete rootItem;
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    auto  _rootItem = new TreeItem(rootData);
    setupModelData(_rootItem,list);
    endResetModel();
}

QModelIndex FolderViewAbstractModel::index(int row, int column, const QModelIndex &parent = QModelIndex()) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FolderViewAbstractModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FolderViewAbstractModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->childCount();
    else
        return rootItem->childCount();
}

int FolderViewAbstractModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant FolderViewAbstractModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(0);
    }
    if ( role == Qt::DecorationRole ) {
        QFileIconProvider fileIcon;
       return fileIcon.icon(QFileInfo(index.data().toString()));
    }
    if(role == 0x0100)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(1);
    }
    return QVariant();

}

void FolderViewAbstractModel::setupModelData(TreeItem *parent, const QList<QString> data)
{
    rootItem = parent;
    for(const auto &path : data)
    {
        QList<QVariant> var;
        if (!QDir(path).exists()) {
            continue;
        }

        buildTree(rootItem,path);
    }
}
void FolderViewAbstractModel::buildTree(TreeItem *parent,QString path)
{
    TreeItem* newParent = parent;
    QDir dir(path);
    dir.setFilter(QDir::Files |  QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QList<QVariant> var;
    var << dir.dirName();
    var << dir.absolutePath();
    auto treeItem = new TreeItem(var,parent);
    parent->appendChild(treeItem);
    newParent = treeItem;
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        if (list.at(i).isDir()) {
            buildTree(newParent,list.at(i).absoluteFilePath());
        }
        else
        {
            if (list.at(i).absoluteFilePath().contains(".dll")) {
                QList<QVariant> var2;
                var2 << list.at(i).fileName();
                var2 << list.at(i).absoluteFilePath();
                auto treeItem = new TreeItem(var2,parent);
                parent->appendChild(treeItem);
            }
        }
    }
}



TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parentItem)
{
    m_parentItem = parentItem;
    m_itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *child)
{
    m_childItems.append(child);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.size();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}
