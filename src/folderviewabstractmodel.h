#ifndef FOLDERVIEWABSTRACTMODEL_H
#define FOLDERVIEWABSTRACTMODEL_H
class TreeItem;
#include <QObject>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QListWidgetItem>

class FolderViewAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FolderViewAbstractModel(const QList<QString> data, QObject *parent = 0);
    void setModel(QList<QString> list);
protected:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
    void setupModelData(TreeItem *parent, const QList<QString> data);
    void buildTree(TreeItem *parent, QString path);
    TreeItem *rootItem;
};

#endif // FOLDERVIEWABSTRACTMODEL_H

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};
