#ifndef Knotrenderer_primitive_HEADER
#define Knotrenderer_primitive_HEADER

#include "Knotrenderer.h"

class KnotRendererPrimitive: public KnotRenderer
{
    virtual void drawText(int x, int y, int fonttype, int fontsize,
        int align, int colour, char *text);
    virtual void drawRect(int x, int y, int w, int h, int colour);
    virtual void drawLine(int x1, int y1, int x2, int y2,
                int colour);
    virtual void drawPolygon(int *coords, int npoints,
        int fillcolour, int outlinecolour);
    virtual void drawCircle(int cx, int cy, int radius,
        int fillcolour, int outlinecolour);
    virtual void drawUpdate(int x, int y, int w, int h);
    virtual void clip(int x, int y, int w, int h);
    virtual void unclip();
    virtual void startDraw();
    virtual void endDraw();
    virtual void drawThickLine(float thickness,
        float x1, float y1, float x2, float y2,
        int colour);
    
    virtual ~KnotRendererPrimitive();
};

#endif