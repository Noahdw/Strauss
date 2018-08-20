#ifndef PLUGINTRACKVIEW_H
#define PLUGINTRACKVIEW_H

class TrackView;
class PluginView;
class PluginEditorContainer;

#include <QObject>
#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include "src/vst2hostcallback.h"

class PluginTrackView : public QFrame
{
    Q_OBJECT
public:
    PluginTrackView(TrackView *track);
    ~PluginTrackView();
    std::vector<PluginView*> plugins;
    void addPlugin(pluginHolder *holder);
    void clickedOn(bool state);

    PluginEditorContainer * plugin_editor_container;
    TrackView * trackView;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:

    QLineEdit *instrumentLabel;
    QCheckBox *muteBox;
    QCheckBox *recordBox;
    QVBoxLayout *vlayout;
    QBrush brush;
    QPen pen;
    int red;
    int green;
    int blue;

};

#endif // PLUGINTRACKVIEW_H
