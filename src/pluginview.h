#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QFrame>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include "src/vst2hostcallback.h"
#include <QLabel>

class PluginView : public QFrame
{
    Q_OBJECT
public:
    PluginView(pluginHolder *pholder);
    void showPlugin();
    void disablePlugin(bool state);
    pluginHolder *holder;
protected:
    void paintEvent(QPaintEvent *event);
private:
    QVBoxLayout *vLayout;
    Vst2HostCallback * plugin;
    QString pluginName;
    QLabel * pluginNameLabel;
    QPushButton *showButton;
    QPushButton *disableButton;
};

#endif // PLUGINVIEW_H
