#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <Plasma/Theme>

#include "Knotrenderer-batch.h"

KnotRendererBatch::KnotRendererBatch(QGraphicsItem* parent, Qt::WindowFlags wFlags):KnotRenderer(parent, wFlags)
    , m_paint_interface(NULL)
{
}

KnotRendererBatch::~KnotRendererBatch()
{
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

void KnotRendererBatch::drawText(int x, int y, bool monospace, int fontsize,
     int align, int colour, const QString& text)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchTextAction(x,y,monospace,fontsize,align,colour,text));
    }
}

void KnotRendererBatch::drawRect(int x, int y, int w, int h, int colour)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchRectAction(x,y,w,h,colour));
    }
}

void KnotRendererBatch::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchLineAction(x1,y1,x2,y2,colour));
    }
}

void KnotRendererBatch::drawPolygon(const QPolygon& polygon,
     int fillcolour, int outlinecolour)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchPolyAction(polygon,fillcolour,outlinecolour));
    }
}

void KnotRendererBatch::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchCircleAction(cx,cy,radius,fillcolour,outlinecolour));
    }
}

void KnotRendererBatch::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
    if (m_paint_interface != NULL)
    {
        m_batch.append(new KnotBatchThickAction(thickness,x1,y1,x2,y2,colour));
    }
}

void KnotRendererBatch::drawUpdate(int x, int y, int w, int h)
{
    if (m_paint_interface != NULL)
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
    if (m_paint_interface != NULL)
    {
        m_paint_interface->p->setClipRect(QRectF(x,y,w,h));
        m_paint_interface->p->setClipping(true);
    }
}

void KnotRendererBatch::unclip()
{
    if (m_paint_interface != NULL)
    {
        m_paint_interface->p->setClipping(false);
    }
}

void KnotRendererBatch::startDraw()
{
    if (m_paint_interface != NULL)
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
    m_color_list = colorList;
}


void KnotRendererBatch::paintInterface(QPainter *p,
    const QStyleOptionGraphicsItem *option,
    const QRectF& contentsRect)
{
    /*
     * Cannot find a themeChanged event. Just change color every time we paint.
     */
    m_paint_interface = new PaintInterfaceData(p, option);
 
    p->save();
    p->translate(getOffset());
    emit forceRedrawRequest();
    p->restore();
    
    /*
     * Preprocess the batch
     */
    preprocessBatch(m_color_list);
    
    /*
     * Run the batch
     */
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        (*it)->apply(m_paint_interface, m_color_list);
    
    /*
     * Delete the batch
     */
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        delete (*it);
    m_batch.clear();

    delete m_paint_interface;
    m_paint_interface = NULL;
}

void KnotRendererBatch::preprocessBatch(QList<QColor> colorList)
{
}

void KnotRendererBatch::PaintInterfaceData::set(int fillColour, int outlineColour, int outlineWidth, const QList< QColor > colorList)
{
    QPen pen = p->pen();
    pen.setWidth(outlineWidth);
    pen.setColor(colorList[outlineColour]);
    p->setPen(pen);
    

    QBrush brush = p->brush();
    if (fillColour == -1)
    {
        brush.setStyle(Qt::NoBrush);
    }
    else
    {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(colorList[fillColour]);
    }
    p->setBrush(brush);
}

void KnotRendererBatch::KnotBatchTextAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(colour, colour, 0, color_list);
    paint_interface->p->setFont(QFont(monospace? "monospace" : "sans", fontsize));
    /*
     * We transform the sane syntax of SGT to the less sane syntax of QPainter
     * By generating an arbitrary large rectangle that will surely contain everything
     */
    qreal largeDimension = 1e5;
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
        paint_interface->p->drawText(rect, flag, text);
    }
    else
    {
        rect = paint_interface->p->boundingRect(rect, flag, text);
        paint_interface->p->drawText(rect.left(), y, text);
    }
}

void KnotRendererBatch::KnotBatchRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(colour, colour, 0, color_list);
    paint_interface->p->drawRect(QRectF(x,y,w,h));
}

void KnotRendererBatch::KnotBatchLineAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}

void KnotRendererBatch::KnotBatchCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}

void KnotRendererBatch::KnotBatchPolyAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}

void KnotRendererBatch::KnotBatchClipAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}

void KnotRendererBatch::KnotBatchUnclipAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}

void KnotRendererBatch::KnotBatchThickAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{

}


#include "Knotrenderer-batch.moc"