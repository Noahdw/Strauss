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
#include <QSlider>
class PluginTrackView : public QFrame
{
    Q_OBJECT
public:
    PluginTrackView(TrackView *track);
    ~PluginTrackView();
    std::vector<PluginView*> plugins;
    void addPlugin(pluginHolder *holder);
    void clickedOn(bool state);
    void setTrackName(QString name);
    PluginEditorContainer * plugin_editor_container;
    TrackView * trackView;
public slots:
    void volumeChanged();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QSlider *volumeSlider;
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
