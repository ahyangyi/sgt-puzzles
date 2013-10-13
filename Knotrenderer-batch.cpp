#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <Plasma/Theme>

#include <cstdio>

#include "Knotrenderer-batch.h"
#include "Knotdebug.h"

KnotRendererBatch::KnotRendererBatch(QGraphicsItem* parent, Qt::WindowFlags wFlags):KnotRenderer(parent, wFlags)
    , m_paint_interface(nullptr)
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
    geometryChangedHandler();
}

void KnotRendererBatch::drawText(int x, int y, bool monospace, int fontsize,
     int align, int colour, const QString& text)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchTextAction(x,y,monospace,fontsize,align,colour,text)));
    }
}

void KnotRendererBatch::drawRect(int x, int y, int w, int h, int colour)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchRectAction(x,y,w,h,colour)));
    }
}

void KnotRendererBatch::drawLine(int x1, int y1, int x2, int y2,
     int colour)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchLineAction(x1,y1,x2,y2,colour)));
    }
}

void KnotRendererBatch::drawPolygon(const QPolygon& polygon,
     int fillcolour, int outlinecolour)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchPolyAction(polygon,fillcolour,outlinecolour)));
    }
}

void KnotRendererBatch::drawCircle(int cx, int cy, int radius,
     int fillcolour, int outlinecolour)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchCircleAction(cx,cy,radius,fillcolour,outlinecolour)));
    }
}

void KnotRendererBatch::drawThickLine(float thickness,
     float x1, float y1, float x2, float y2,
     int colour)
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchThickAction(thickness,x1,y1,x2,y2,colour)));
    }
}

void KnotRendererBatch::drawUpdate(int x, int y, int w, int h)
{
    if (m_paint_interface != nullptr)
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
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchClipAction(x,y,w,h)));
    }
}

void KnotRendererBatch::unclip()
{
    if (m_paint_interface != nullptr)
    {
        m_batch.append(std::shared_ptr<KnotBatchAction>(new KnotBatchUnclipAction()));
    }
}

void KnotRendererBatch::startDraw()
{
    if (m_paint_interface != nullptr)
    {
    }
}

void KnotRendererBatch::endDraw()
{
    if (m_paint_interface != nullptr)
    {
    }
}

void KnotRendererBatch::notStarted()
{
    if (m_paint_interface != nullptr)
    {
    }
}

void KnotRendererBatch::setColor(QList< QColor > colorList)
{
    m_color_list = colorList;
}


void KnotRendererBatch::paintInterface(QPainter *p,
    const QStyleOptionGraphicsItem *option,
    const QRectF& contentsRect)
{
    p->save();
    p->translate(getOffset());

    /*
     * Cannot find a themeChanged event. Just change color every time we paint.
     */
    m_paint_interface = new PaintInterfaceData(p, option);
    m_batch.clear();
    emit forceRedrawRequest();
    
    /*
     * Preprocess the batch
     */
    QList<std::shared_ptr<KnotBatchAction>> drawbatch = m_batch;
    preprocessBatch(drawbatch);
    
    /*
     * Run the batch
     */
    for (auto it = drawbatch.begin(); it != drawbatch.end(); ++it)
        (*it)->apply(m_paint_interface, m_color_list);

    delete m_paint_interface;
    m_paint_interface = nullptr;

    p->restore();
}

void KnotRendererBatch::preprocessBatch(QList<std::shared_ptr<KnotBatchAction>>& m_batch)
{
    /*
     * Intentionally left blank
     */
}

void KnotRendererBatch::PaintInterfaceData::set(int fillColour, int outlineColour, int outlineWidth, const QList< QColor > colorList)
{
    QPen pen = p->pen();
    pen.setWidth(outlineWidth);
    if (outlineColour != -1)
        pen.setColor(colorList[outlineColour]);
    else
    {
        QBrush brush = pen.brush();
        brush.setStyle(Qt::NoBrush);
        pen.setBrush(brush);
    }
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


#include "Knotrenderer-batch.moc"