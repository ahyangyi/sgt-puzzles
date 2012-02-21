#include <QPainter>

#include "Knotrenderer-primitive.h"
#include <Plasma/Theme>

struct PaintInterfaceData
{
    QPainter *p;
    const QStyleOptionGraphicsItem *option;
    QPointF offset;
    
    PaintInterfaceData () {}
    PaintInterfaceData (QPainter *n_p, const QStyleOptionGraphicsItem *n_option,  const QPointF& n_offset):
        p(n_p), option(n_option), offset(n_offset){}
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

void KnotRendererPrimitive::drawText(int x, int y, int fonttype, int fontsize,
     int align, int colour, const QString& text)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->drawText(QPointF(x,y)+d->m_paint_interface->offset, text);
    }
}

void KnotRendererPrimitive::drawRect(int x, int y, int w, int h, int colour)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setPen(QPen());
        d->m_paint_interface->p->setBrush(QBrush(d->m_color_list[colour]));
        d->m_paint_interface->p->drawRect(QRectF(x,y,w,h).translated(d->m_paint_interface->offset));
    }
}

void KnotRendererPrimitive::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setPen(QPen(d->m_color_list[colour]));
        d->m_paint_interface->p->drawLine(QPointF(x1,y1)+d->m_paint_interface->offset, QPointF(x2,y2)+d->m_paint_interface->offset);
    }
}

void KnotRendererPrimitive::drawPolygon(const QPolygon& polygon,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setPen(QPen(d->m_color_list[outlinecolour]));
        d->m_paint_interface->p->setBrush(QBrush(d->m_color_list[fillcolour]));
        d->m_paint_interface->p->drawPolygon(QPolygonF(polygon).translated(d->m_paint_interface->offset));
    }
}

void KnotRendererPrimitive::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setPen(QPen(d->m_color_list[outlinecolour]));
        d->m_paint_interface->p->setBrush(QBrush(d->m_color_list[fillcolour]));
        d->m_paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2).translated(d->m_paint_interface->offset));
    }
}

void KnotRendererPrimitive::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->save();
        d->m_paint_interface->p->setPen(QPen(d->m_color_list[colour], thickness));
        d->m_paint_interface->p->drawLine(QPointF(x1,y1)+d->m_paint_interface->offset, QPointF(x2,y2)+d->m_paint_interface->offset);
        d->m_paint_interface->p->restore();
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
    int x = int(contentsRect.width()), y = int(contentsRect.height());
    
    emit sizeRequest(&x, &y);
    d->m_paint_interface = new PaintInterfaceData(p,option,contentsRect.center()-QPointF(x/2.0, y/2.0));
    
    emit colorRequest(Plasma::Theme::defaultTheme()->color(Plasma::Theme::ViewBackgroundColor));
    
    emit redrawRequest();
/*    
    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            m_svg[i][j].resizeFrame(contentsRect.size() / 4);
            m_svg[i][j].paintFrame(p, contentsRect.topLeft() + QPointF(contentsRect.size().width() / 4 * i, contentsRect.size().height() / 4 * j));
        }
    }
*/
    delete d->m_paint_interface;
    d->m_paint_interface = NULL;
}

#include "Knotrenderer-primitive.moc"