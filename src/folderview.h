#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileSystemModel>
#include <QListView>
#include "pianorollcontainer.h"
class FolderView : public QWidget
{
    Q_OBJECT
public:
    FolderView();
    PianoRollContainer * pRollContainer;
private:
    QVBoxLayout *vLayout;
    QListView *list;
    QString path = "C:/Program Files/Native Instruments/VSTPlugins 64 bit/";

private slots:
   void itemDoubleClicked();
};

#endif // FOLDERVIEW_H
