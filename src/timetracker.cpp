#include "timetracker.h"
#include "src/common.h"

//DEPRECATED CLASS

TimeTracker::TimeTracker()
{
    g_timer->setFrameRange(0,100);
    g_timer->setCurveShape(QTimeLine::LinearCurve);
    g_timer->setUpdateInterval(20);
}
