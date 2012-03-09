#include <Knottimer.h>

KnotTimer::KnotTimer(QObject* parent): QTimer(parent)
{

}
KnotTimer::~KnotTimer()
{

}

void KnotTimer::timerEvent(QTimerEvent* e)
{
    QTimer::timerEvent(e);
    emit tick(interval() / 1000.0);
}

#include "Knottimer.moc"