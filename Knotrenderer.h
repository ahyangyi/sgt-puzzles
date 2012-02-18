#ifndef Knotrenderer_HEADER
#define Knotrenderer_HEADER

#include <QGraphicsWidget>

class KnotRenderer: public QGraphicsWidget
{
public:
    virtual void drawText(int x, int y, int fonttype, int fontsize,
        int align, int colour, char *text) = 0;
    virtual void drawRect(int x, int y, int w, int h, int colour) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2,
                int colour) = 0;
    virtual void drawPolygon(int *coords, int npoints,
        int fillcolour, int outlinecolour) = 0;
    virtual void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour) = 0;
    virtual void drawUpdate(int x, int y, int w, int h) = 0;
    virtual void clip(int x, int y, int w, int h) = 0;
    virtual void unclip() = 0;
    virtual void startDraw() = 0;
    virtual void endDraw() = 0;
    virtual void drawThickLine(float thickness,
        float x1, float y1, float x2, float y2,
        int colour) = 0;
        
    virtual ~KnotRenderer ();
};

#endif