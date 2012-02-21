#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "Knotrenderer.h"

KnotRenderer::KnotRenderer(QGraphicsItem* parent, Qt::WindowFlags wFlags): QGraphicsWidget(parent, wFlags)
{
    setPreferredSize(256,256); //Magical numbers. I guess it is a reasonable size for most screens.
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

KnotRenderer::~KnotRenderer()
{

}

void KnotRenderer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    paintInterface(painter, option, QRectF(QPointF(0,0), boundingRect().size()));
}

void KnotRenderer::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    emit mousePressEvent(e->pos(), e->button());
}

void KnotRenderer::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    emit mouseReleaseEvent(e->pos(), e->button());
}

#include "Knotrenderer.moc"