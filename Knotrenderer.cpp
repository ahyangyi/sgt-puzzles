#include <QWidget>
#include <QPainter>

#include "Knotrenderer.h"

KnotRenderer::KnotRenderer(QGraphicsItem* parent, Qt::WindowFlags wFlags): QGraphicsWidget(parent, wFlags)
{
    setMinimumSize(256,256); //Magical numbers. I guess it is a reasonable size for most screens.
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

KnotRenderer::~KnotRenderer()
{

}

void KnotRenderer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);
    paintInterface(painter, option, QRectF(QPointF(0,0), boundingRect().size()));
}

#include "Knotrenderer.moc"