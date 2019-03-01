#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QFrame>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include "src/vst2audioplugin.h"
#include <QLabel>

class PluginView : public QFrame
{
    Q_OBJECT
public:
    PluginView(Vst2AudioPlugin *vst2Plugin);
    void showPlugin();
    void disablePlugin(bool state);
    Vst2AudioPlugin *plugin;
private:
    QVBoxLayout *vLayout;
    QString pluginName;
    QLabel * pluginNameLabel;
    QPushButton *showButton;
    QPushButton *disableButton;
};

#endif // PLUGINVIEW_H
