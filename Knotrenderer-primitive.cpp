#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QFont>
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

void KnotRendererPrimitive::themeChangedHandler()
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

void KnotRendererPrimitive::setPainter(int fillColour, int outlineColour, int outlineWidth)
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

void KnotRendererPrimitive::drawText(int x, int y, bool monospace, int fontsize,
     int align, int colour, const QString& text)
{
    if (d->m_paint_interface != NULL)
    {
        setPainter(colour, colour, 0);
        d->m_paint_interface->p->setFont(QFont(monospace? "monospace" : "sans", fontsize));
        /*
         * We transform the sane syntax of SGT to the less sane syntax of QPainter
         * By generating an arbitrary large rectangle that will surely contain everything
         */
        qreal largeDimension = qMax(contentsRect().width() * 2 + 100, contentsRect().height() * 2 + 100);
        QRectF rect;
        int flag;
        
#define ALIGN_VNORMAL 0x000
#define ALIGN_VCENTRE 0x100

#define ALIGN_HLEFT   0x000
#define ALIGN_HCENTRE 0x001
#define ALIGN_HRIGHT  0x002

        if (align & ALIGN_HLEFT)
        {
            rect = QRectF(
                x,
                y - largeDimension / 2,
                largeDimension,
                largeDimension
               );
            flag = Qt::AlignLeft|Qt::AlignVCenter; 
        }
        if (align & ALIGN_HCENTRE)
        {
            rect = QRectF(
                x - largeDimension / 2,
                y - largeDimension / 2,
                largeDimension,
                largeDimension
               );
            flag = Qt::AlignHCenter|Qt::AlignVCenter; 
        }
        if (align & ALIGN_HRIGHT)
        {
            rect = QRectF(
                x - largeDimension,
                y - largeDimension / 2,
                largeDimension,
                largeDimension
               );
            flag = Qt::AlignRight|Qt::AlignVCenter; 
        }
        
        if (align & ALIGN_VCENTRE)
        {
            d->m_paint_interface->p->drawText(rect, flag, text);
        }
        else
        {
            rect = d->m_paint_interface->p->boundingRect(rect, flag, text);
            d->m_paint_interface->p->drawText(rect.left(), y, text);
        }
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
    if (d->m_paint_interface != NULL)
    {
    }
    else
    {
        update();
    }
}

void KnotRendererPrimitive::updateAll()
{
    update ();
}

void KnotRendererPrimitive::clip(int x, int y, int w, int h)
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setClipRect(QRectF(x,y,w,h));
        d->m_paint_interface->p->setClipping(true);
    }
}

void KnotRendererPrimitive::unclip()
{
    if (d->m_paint_interface != NULL)
    {
        d->m_paint_interface->p->setClipping(false);
    }
}

void KnotRendererPrimitive::startDraw()
{
    if (d->m_paint_interface != NULL)
    {
    }
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
    /*
     * Cannot find a themeChanged event. Just change color every time we paint.
     */
    d->m_paint_interface = new PaintInterfaceData(p, option);
 
    p->save();
    p->translate(getOffset());
    emit forceRedrawRequest();
    p->restore();

    delete d->m_paint_interface;
    d->m_paint_interface = NULL;
}

#include "Knotrenderer-primitive.moc"