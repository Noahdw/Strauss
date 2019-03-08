#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QTableWidget>

class FolderView;
class FolderViewAbstractModel;
class MasterTrack;
class Browser : public QFrame
{
public:
    Browser(FolderViewAbstractModel *qmodel, MasterTrack *master);
private:
    QTabWidget* tabWidget;
    FolderView* folderView;
    QVBoxLayout* mainLayout;
};

#endif // BROWSER_H
