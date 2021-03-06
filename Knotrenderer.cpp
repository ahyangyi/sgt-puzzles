#include <QWidget>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QButtonGroup>
#include <QKeyEvent>

#include <KDE/Plasma/Theme>

#include "Knotrenderer.h"
#include "Knotdebug.h"

KnotRenderer::KnotRenderer(QGraphicsItem* parent, Qt::WindowFlags wFlags): QGraphicsWidget(parent, wFlags)
{
    setPreferredSize(256,256); //Magical numbers. I guess it is a reasonable size for most screens.
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    updateGeometry();
    setFlag(ItemIsFocusable, true);
    
    m_initialized = false;
    
    connect(this,SIGNAL(geometryChanged()),this,SLOT(geometryChangedHandler()));
    connect(Plasma::Theme::defaultTheme(),SIGNAL(themeChanged()),this,SLOT(themeChangedHandler()));
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
    setFocus();
    QGraphicsItem::mousePressEvent(e);
    if (contentsRect().contains(e->pos()))
        emit mousePressed((e->pos()-getOffset()).toPoint(), e->button());
    e->accept();
}

void KnotRenderer::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsItem::mouseReleaseEvent(e);
    if (contentsRect().contains(e->pos()))
        emit mouseReleased((e->pos()-getOffset()).toPoint(), e->button());
    e->accept();
}

void KnotRenderer::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsItem::mouseMoveEvent(e);
    if (e->buttons() & (Qt::LeftButton | Qt::MiddleButton | Qt::RightButton))
    {
        emit mouseDragged((e->pos()-getOffset()).toPoint(), e->buttons());
        e->accept();
    }
}

void KnotRenderer::keyPressEvent(QKeyEvent* e)
{
    QGraphicsItem::keyPressEvent(e);
    
    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Left || e->key() == Qt::Key_Right || e->key() == Qt::Key_Return ||
        e->key() == Qt::Key_0 || e->key() == Qt::Key_1 || e->key() == Qt::Key_2 ||  e->key() == Qt::Key_3 || e->key() == Qt::Key_4 ||  
        e->key() == Qt::Key_5 || e->key() == Qt::Key_6 || e->key() == Qt::Key_7 ||  e->key() == Qt::Key_8 || e->key() == Qt::Key_9 ||
        e->key() >= Qt::Key_Space && e->key() <= 0xff ||
        e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete ||
        false
    )
    {
        emit keyPressed(e->key(), e->modifiers());
        e->accept();
    }
}

void KnotRenderer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    paintInterface(painter, option, contentsRect());
    painter->restore();
}

void KnotRenderer::geometryChangedHandler()
{
    if (!m_initialized)
        return;
    int x = int(contentsRect().width()), y = int(contentsRect().height());

    emit sizeRequest(&x, &y);
    
    setOffset(contentsRect().center() - QPointF(x / 2., y / 2.));
}

void KnotRenderer::themeChangedHandler()
{
    geometryChangedHandler();
    emit colorRequest(Plasma::Theme::defaultTheme()->color(Plasma::Theme::ViewBackgroundColor));
}

void KnotRenderer::configChangedHandler(KConfigGroup cg)
{
    themeChangedHandler();
    m_initialized = true;
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