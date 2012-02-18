#include "Knotrenderer-primitive.h"

void KnotRendererPrimitive::drawText(int x, int y, int fonttype, int fontsize,
     int align, int colour, char *text)
{
}

void KnotRendererPrimitive::drawRect(int x, int y, int w, int h, int colour)
{
}

void KnotRendererPrimitive::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
}
void KnotRendererPrimitive::drawPolygon(int *coords, int npoints,
     int fillcolour, int outlinecolour)
{
}

void KnotRendererPrimitive::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
}

void KnotRendererPrimitive::drawUpdate(int x, int y, int w, int h)
{
}

void KnotRendererPrimitive::clip(int x, int y, int w, int h)
{
}

void KnotRendererPrimitive::unclip()
{
}

void KnotRendererPrimitive::startDraw()
{
}

void KnotRendererPrimitive::endDraw()
{
}

void KnotRendererPrimitive::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
}

KnotRendererPrimitive::~KnotRendererPrimitive()
{

}
