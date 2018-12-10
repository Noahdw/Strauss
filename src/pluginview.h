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
    PluginView(Vst2HostCallback *vst2Plugin);
    void showPlugin();
    void disablePlugin(bool state);
    Vst2HostCallback *plugin;
private:
    QVBoxLayout *vLayout;
    QString pluginName;
    QLabel * pluginNameLabel;
    QPushButton *showButton;
    QPushButton *disableButton;
};

#endif // PLUGINVIEW_H
