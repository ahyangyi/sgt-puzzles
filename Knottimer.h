#ifndef Knottimer_HEADER
#define Knottimer_HEADER
#include<QTimer>

class KnotTimer: public QTimer
{
    Q_OBJECT
public:
    KnotTimer (QObject  *parent=0 );
    ~KnotTimer ();
    void timerEvent (QTimerEvent  *e );
signals:
    void tick (qreal time);
};

#endif