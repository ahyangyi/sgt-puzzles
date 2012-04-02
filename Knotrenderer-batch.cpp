#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <Plasma/Theme>

#include "Knotrenderer-batch.h"

struct PaintInterfaceData
{
    QPainter *p;
    const QStyleOptionGraphicsItem *option;
    
    PaintInterfaceData () {}
    PaintInterfaceData (QPainter *n_p, const QStyleOptionGraphicsItem *n_option):
        p(n_p), option(n_option){}
};

class KnotRendererBatch::Private
{
public:
    PaintInterfaceData* m_paint_interface;
    QList<QColor> m_color_list;
};

KnotRendererBatch::KnotRendererBatch(QGraphicsItem* parent, Qt::WindowFlags wFlags):KnotRenderer(parent, wFlags)
    ,d(new Private)
{
    d->m_paint_interface = NULL;
}

KnotRendererBatch::~KnotRendererBatch()
{
    delete d;
}

void KnotRendererBatch::themeChangedHandler()
{
    /*
     * We exlude colors that are too dark (but light colors are probably OK)
     * lighter() multiplies a factor to the value, so it's not good enough to deal with potential pure black
     * Force the value to be at least 0.2 seems to make more sense
     */
    
    QColor color = Plasma::Theme::defaultTheme()->color(Plasma::Theme::ViewBackgroundColor);
    if (color.valueF() < 0.2)
        color.setHsvF(color.hueF(), color.saturationF(), 0.2);
    
    emit colorRequest(color);
}

void KnotRendererBatch::setPainter(int fillColour, int outlineColour, int outlineWidth)
{
    QPen pen = d->m_paint_interface->p->pen();
    pen.setWidth(outlineWidth);
    pen.setColor(d->m_color_list[outlineColour]);
    d->m_paint_interface->p->setPen(pen);
    

    QBrush brush = d->m_paint_interface->p->brush();
    if (fillColour == -1)
    {
        brush.setStyle(Qt::NoBrush);
    }
    else
    {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(d->m_color_list[fillColour]);
    }
    d->m_paint_interface->p->setBrush(brush);
}

void KnotRendererBatch::drawText(int x, int y, bool monospace, int fontsize,
     int align, int colour, const QString& text)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchTextAction(x,y,monospace,fontsize,align,colour,text));
    }
}

void KnotRendererBatch::drawRect(int x, int y, int w, int h, int colour)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchRectAction(x,y,w,h,colour));
    }
}

void KnotRendererBatch::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchLineAction(x1,y1,x2,y2,colour));
    }
}

void KnotRendererBatch::drawPolygon(const QPolygon& polygon,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchPolyAction(polygon,fillcolour,outlinecolour));
    }
}

void KnotRendererBatch::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchCircleAction(cx,cy,radius,fillcolour,outlinecolour));
    }
}

void KnotRendererBatch::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
    if (d->m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchThickAction(thickness,x1,y1,x2,y2,colour));
    }
}

void KnotRendererBatch::drawUpdate(int x, int y, int w, int h)
{
    if (d->m_paint_interface != NULL)
    {
    }
    else
    {
        update();
    }
}

void KnotRendererBatch::updateAll()
{
    update ();
}

void KnotRendererBatch::clip(int x, int y, int w, int h)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setClipRect(QRectF(x,y,w,h));
        d->m_paint_interface->p->setClipping(true);
    }
}

void KnotRendererBatch::unclip()
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setClipping(false);
    }
}

void KnotRendererBatch::startDraw()
{
    if (d->m_paint_interface != NULL)
    {
    }
}

void KnotRendererBatch::endDraw()
{
}

void KnotRendererBatch::notStarted()
{

}

void KnotRendererBatch::setColor(QList< QColor > colorList)
{
    d->m_color_list = colorList;
}


void KnotRendererBatch::paintInterface(QPainter *p,
    const QStyleOptionGraphicsItem *option,
    const QRectF& contentsRect)
{
    /*
     * Cannot find a themeChanged event. Just change color every time we paint.
     */
    d->m_paint_interface = new PaintInterfaceData(p, option);
 
    p->save();
    p->translate(getOffset());
    emit forceRedrawRequest();
    p->restore();
    
    processBatch();

    delete d->m_paint_interface;
    d->m_paint_interface = NULL;
}

void KnotRendererBatch::processBatch()
{
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
    {
        if (it->getType() == "rect")
            
    }
}

#include "Knotrenderer-batch.moc"