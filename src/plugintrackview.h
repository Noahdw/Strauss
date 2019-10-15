#ifndef PLUGINTRACKVIEW_H
#define PLUGINTRACKVIEW_H

class TrackWidget;
class PluginView;
class PluginEditorContainer;
class TrackMidi;

#include <QObject>
#include <QFrame>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include "src/vst2audioplugin.h"
#include <QSlider>

class PluginTrackView : public QFrame
{
    Q_OBJECT
public:
    PluginTrackView(TrackWidget *track);
    ~PluginTrackView();
    std::vector<PluginView*> plugins;
    void addPlugin(Vst2AudioPlugin *vst2Plugin);
    void clickedOn(bool state);
    void setTrackName(QString name);
    PluginEditorContainer * pluginEditorContainer;
    TrackWidget * trackView;
    TrackMidi *midiTrack;

public slots:
    void panChanged();
    void volumeChanged();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QSlider *volumeSlider;
    QSlider *panSlider;
    QLineEdit *instrumentLabel;
    QCheckBox *muteBox;
    QCheckBox *recordBox;
    QVBoxLayout *vlayout;
    QBrush brush;
    QPen pen;

    int red;
    int green;
    int blue;
    int pan = 64;
};

#endif // PLUGINTRACKVIEW_H
