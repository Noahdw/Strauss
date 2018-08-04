#ifndef COMMON_H
#define COMMON_H
#include <QTimeLine>

extern bool keyboardModeEnabled;
extern double g_tempo;
extern double g_quarterNotes;
extern QTimeLine *g_timer;
extern int g_blocksize;
extern int g_sampleRate;
extern double g_volume;
#endif // COMMON_H
