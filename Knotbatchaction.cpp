#include "Knotrenderer-batch.h"
#include <QPainter>

/*
 * Rectangles
 */
void KnotRendererBatch::KnotBatchRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
//    paint_interface->set(colour, colour, 0, color_list);
//    paint_interface->p->drawRect(QRectF(x - .5, y - .5, w, h));
    paint_interface->set(colour, -1, 0, color_list);
    paint_interface->p->drawRect(QRect(x, y, w, h));
}

/*
 * Lines
 */

void KnotRendererBatch::KnotBatchLineAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(colour, colour, 1, color_list);
    paint_interface->p->drawLine(QPointF(x1,y1), QPointF(x2,y2));
}

/*
 * Circles
 */

void KnotRendererBatch::KnotBatchCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(fillColour, outlineColour, 1, color_list);
    paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2));
}

QString KnotRendererBatch::KnotBatchPolyAction::toString ()
{
    QString re = "Polygon";
    
    for (QPolygon::iterator it = polygon.begin(); it != polygon.end(); ++it)
    {
        re = re + QString(" (%1,%2)").arg(it->x()).arg(it->y());
    }
    re += QString(" Fill %1 Outline %2").arg(fillColour).arg(outlineColour);
    
    return re;
}

void KnotRendererBatch::KnotBatchPolyAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(fillColour, outlineColour, 1, color_list);
    paint_interface->p->drawPolygon(polygon);
}

void KnotRendererBatch::KnotBatchTextAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(colour, colour, 0, color_list);
    paint_interface->p->setFont(QFont(monospace? "monospace" : "sans", fontsize));
    /*
     * We transform the sane syntax of SGT to the less sane syntax of QPainter
     * By generating an arbitrary large rectangle that will surely contain everything
     */
    qreal largeDimension = 5e4;
    QRectF rect;
    int flag;
    
#define ALIGN_VNORMAL 0x000
#define ALIGN_VCENTRE 0x100

#define ALIGN_HLEFT   0x000
#define ALIGN_HCENTRE 0x001
#define ALIGN_HRIGHT  0x002
#define ALIGN_HMASK   0x003

    if ((align & ALIGN_HMASK) == ALIGN_HLEFT)
    {
        rect = QRectF(
            x,
            y - largeDimension / 2,
            largeDimension,
            largeDimension
            );
        flag = Qt::AlignLeft|Qt::AlignVCenter; 
    }
    if ((align & ALIGN_HMASK) == ALIGN_HCENTRE)
    {
        rect = QRectF(
            x - largeDimension / 2,
            y - largeDimension / 2,
            largeDimension,
            largeDimension
            );
        flag = Qt::AlignHCenter|Qt::AlignVCenter; 
    }
    if ((align & ALIGN_HMASK) == ALIGN_HRIGHT)
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

void KnotRendererBatch::KnotBatchClipAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->p->setClipRect(QRectF(x,y,w,h));
    paint_interface->p->setClipping(true);
}

void KnotRendererBatch::KnotBatchUnclipAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->p->setClipping(false);
}

void KnotRendererBatch::KnotBatchThickAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(colour, colour, thickness, color_list);
    paint_interface->p->drawLine(QPointF(x1,y1), QPointF(x2,y2));
}
