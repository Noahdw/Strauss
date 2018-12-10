#ifndef COMMON_H
#define COMMON_H
#include <QTimeLine>

extern bool keyboardModeEnabled;
extern double g_tempo;
extern double g_quarterNotes;
extern QTimeLine *g_timer;
extern int g_blocksize;
extern int g_sampleRate;
extern int g_totalDt;
extern double g_volume;
extern QColor g_backgroundColor;
extern QColor g_baseColor;
extern QColor g_selectedColor;
#endif // COMMON_H
