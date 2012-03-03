#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <Plasma/Theme>

#include "Knotrenderer-primitive.h"

struct PaintInterfaceData
{
    QPainter *p;
    const QStyleOptionGraphicsItem *option;
    
    PaintInterfaceData () {}
    PaintInterfaceData (QPainter *n_p, const QStyleOptionGraphicsItem *n_option):
        p(n_p), option(n_option){}
};

class KnotRendererPrimitive::Private
{
public:
    PaintInterfaceData* m_paint_interface;
    QList<QColor> m_color_list;
};

KnotRendererPrimitive::KnotRendererPrimitive(QGraphicsItem* parent, Qt::WindowFlags wFlags):KnotRenderer(parent, wFlags)
    ,d(new Private)
{
    d->m_paint_interface = NULL;
}

KnotRendererPrimitive::~KnotRendererPrimitive()
{
    delete d;
}

void KnotRendererPrimitive::setPainter(int fillColour, int outlineColour, int outlineWidth)
{
    QPen pen = d->m_paint_interface->p->pen();
    pen.setWidth(outlineWidth);
    pen.setColor(d->m_color_list[outlineColour]);
    d->m_paint_interface->p->setPen(pen);
    
/*
 *   QBrush brush = d->m_paint_interface->p->brush();
 *   brush.setColor(d->m_color_list[fillColour]);
 *   d->m_paint_interface->p->setBrush(brush);
 */
    d->m_paint_interface->p->setBrush(QBrush(d->m_color_list[fillColour]));
}

void KnotRendererPrimitive::drawText(int x, int y, int fonttype, int fontsize,
     int align, int colour, const QString& text)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->drawText(QPointF(x,y), text);
    }
}

void KnotRendererPrimitive::drawRect(int x, int y, int w, int h, int colour)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(colour, colour, 0);
        d->m_paint_interface->p->drawRect(QRectF(x,y,w,h));
    }
}

void KnotRendererPrimitive::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(colour, colour, 0);
        d->m_paint_interface->p->drawLine(QPointF(x1,y1), QPointF(x2,y2));
    }
}

void KnotRendererPrimitive::drawPolygon(const QPolygon& polygon,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(fillcolour, outlinecolour, 0);
        d->m_paint_interface->p->drawPolygon(polygon);
    }
}

void KnotRendererPrimitive::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(fillcolour, outlinecolour, 0);
        d->m_paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2));
    }
}

void KnotRendererPrimitive::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(colour, colour, thickness);
        d->m_paint_interface->p->drawLine(QPointF(x1,y1), QPointF(x2,y2));
    }
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

void KnotRendererPrimitive::notStarted()
{

}

void KnotRendererPrimitive::setColor(QList< QColor > colorList)
{
    d->m_color_list = colorList;
}


void KnotRendererPrimitive::paintInterface(QPainter *p,
    const QStyleOptionGraphicsItem *option,
    const QRectF& contentsRect)
{
//    initialize();
    
    d->m_paint_interface = new PaintInterfaceData(p, option);
 
    p->save();
    p->translate(getOffset());
    emit forceRedrawRequest();
    p->restore();

    delete d->m_paint_interface;
    d->m_paint_interface = NULL;
}

#include "Knotrenderer-primitive.moc"