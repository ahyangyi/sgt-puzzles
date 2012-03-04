#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <KDE/Plasma/Theme>

#include "Knotrenderer.h"

KnotRenderer::KnotRenderer(QGraphicsItem* parent, Qt::WindowFlags wFlags): QGraphicsWidget(parent, wFlags)
{
    setPreferredSize(256,256); //Magical numbers. I guess it is a reasonable size for most screens.
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    updateGeometry();
    
    connect(this,SIGNAL(geometryChanged()),this,SLOT(geometryChangedHandler()));
}

KnotRenderer::~KnotRenderer()
{

}

bool KnotRenderer::event(QEvent* event)
{
    if (event->type() == QEvent::ContentsRectChange)
        geometryChangedHandler();
    return QGraphicsWidget::event(event);
}

void KnotRenderer::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    emit mousePressed((e->pos()-getOffset()).toPoint(), e->buttons());
    e->accept();
    this->update();
    QGraphicsItem::mousePressEvent(e);
}

void KnotRenderer::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    emit mouseReleased((e->pos()-getOffset()).toPoint(), e->buttons());
    e->accept();
    this->update();
    QGraphicsItem::mouseReleaseEvent(e);
}

void KnotRenderer::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    if (e->buttons() & (Qt::LeftButton | Qt::MiddleButton | Qt::RightButton))
    {
        emit mouseDragged((e->pos()-getOffset()).toPoint(), e->buttons());
        e->accept();
        this->update();
    }

    QGraphicsItem::mouseMoveEvent(e);
}


void KnotRenderer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    paintInterface(painter, option, contentsRect());
}

void KnotRenderer::geometryChangedHandler()
{
    int x = int(contentsRect().width()), y = int(contentsRect().height());

    emit sizeRequest(&x, &y);
    
    setOffset(contentsRect().center() - QPointF(x / 2., y / 2.));
}

void KnotRenderer::themeChangedHandler()
{
    emit colorRequest(Plasma::Theme::defaultTheme()->color(Plasma::Theme::ViewBackgroundColor));
}

void KnotRenderer::initialize()
{
    geometryChangedHandler();
    themeChangedHandler();
}

QPointF KnotRenderer::getOffset()
{
    return m_offset;
}

void KnotRenderer::setOffset(const QPointF& offset)
{
    m_offset = offset;
}

#include "Knotrenderer.moc"