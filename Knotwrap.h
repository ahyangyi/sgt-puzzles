#ifndef Knotwrap_HEADER
#define Knotwrap_HEADER
#include <qobject.h>

extern "C"
{
    #include "puzzles.h"
    
    /*
     * Mid-end to front-end calls
     */
    void get_random_seed(void **randseed, int *randseedsize);
    void activate_timer(frontend *fe);
    void deactivate_timer(frontend *fe);
    void fatal(char *fmt, ...);
    void frontend_default_colour(frontend *fe, float *output);

    struct frontend
    {
        Knotplasm* plasm;
    };
}

class KnotMidEnd : public QObject
{
public:
    KnotMidEnd(Knotplasm* parent);
    virtual ~KnotMidEnd();
    
    void newGame();
    void size(int* portX, int* portY);
    void redraw();
    void forceRedraw();
private:
    midend* m_me;
};

#endif