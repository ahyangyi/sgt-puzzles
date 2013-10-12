#include <QPainter>

#include <algorithm>
#include "Knotrenderer-plasma.h"
#include "Knotdebug.h"
#include "Knotconfig.h"
#include "Knotplasmagamehandler.h"
#include <cmath>
#include <Plasma/Theme>
#include <typeinfo>

class KnotRendererPlasma::Private
{
public:
    KConfigGroup m_cg;
    
    Plasma::Svg *fifteen;
    Plasma::FrameSvg *round;
    bool roundValid;
    Plasma::FrameSvg *roundFallback;
    bool roundFallbackValid;
};

KnotRendererPlasma::KnotRendererPlasma(QGraphicsItem* parent, Qt::WindowFlags wFlags): KnotRendererPlasma(GameHandlerFactoryImpl::instance(), parent, wFlags)
{
}

KnotRendererPlasma::KnotRendererPlasma(KnotRendererPlasma::GameHandlerFactory* handlerFactory, QGraphicsItem* parent, Qt::WindowFlags wFlags): 
    KnotRendererBatch(parent, wFlags), d(new Private()), m_handler_factory(GameHandlerFactoryImpl::instance()), m_handler(nullptr)
{

}

KnotRendererPlasma::~KnotRendererPlasma()
{

}

void KnotRendererPlasma::themeChangedHandler()
{
    KnotRendererBatch::themeChangedHandler();
    d->roundValid = d->round->isValid();
    d->roundFallbackValid = d->roundFallback->isValid();
}

void KnotRendererPlasma::configChangedHandler(KConfigGroup cg)
{
    d->m_cg = cg;
    
    if (m_handler != nullptr)
        m_handler->free();
    m_handler = m_handler_factory->getGameHandler(cg);
    
    if (!m_initialized)
    {
        d->round = new Plasma::FrameSvg(this);
        d->round->setImagePath("widgets/circular-background");
        d->round->setEnabledBorders(Plasma::FrameSvg::NoBorder);
        d->roundValid = d->round->isValid();
        d->roundFallback = new Plasma::FrameSvg(this);
        d->roundFallback->setImagePath("widgets/translucentbackground");
        //d->roundFallback->setImagePath("widgets/frame");
        //d->roundFallback->setElementPrefix("sunken");
        d->roundFallback->setEnabledBorders(Plasma::FrameSvg::AllBorders);
        d->roundFallbackValid = d->roundFallback->isValid();
        d->fifteen = new Plasma::Svg(this);
        d->fifteen->setImagePath("fifteenPuzzle/blanksquare");
    }
    KnotRenderer::configChangedHandler(cg);
}

void KnotRendererPlasma::geometryChangedHandler()
{
    if (!m_initialized)
        return;
    
    int x = int(contentsRect().width()), y = int(contentsRect().height());
    int cx = x, cy = y;
    
    while (true)
    {
        int rx = cx, ry = cy, ox = 0, oy = 0;
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
            
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = nullptr;
    
        if (rx > x || ry > y)
            break;
        cx *= 2;
        cy *= 2;
    }
    
    int lx = cx/2, ly = cy/2;
    while (lx < cx || ly < cy)
    {
        int rx = (lx+cx+1)/2, ry = (ly+cy+1)/2, ox = 0, oy = 0;
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
            
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = nullptr;

        if (rx > x || ry > y)
        {
            cx = (lx+cx)/2;
            cy = (ly+cy)/2;
        }
        else
        {
            lx = (lx+cx+1)/2;
            ly = (ly+cy+1)/2;
        }
    }
    
    {
        int rx = cx, ry = cy, ox = 0, oy = 0;
        
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
        
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = nullptr;

        setOffset(contentsRect().center() - QPointF(rx / 2., ry / 2.) - QPointF(ox, oy));
    }
}

bool KnotRendererPlasma::contains(const QPointF& point) const
{
    if (m_handler)
    {
//        m_batch.clear();
//        emit forceRedrawRequest();
//        return m_handler->contains(point, m_batch, size());
    }

    return QGraphicsItem::contains(point);
}

void KnotRendererPlasma::getRealDimension(int& x, int& y, int &ox, int &oy)
{
    QString gameName = KnotConfig::getGameName(d->m_cg);

    if (m_handler)
    {
        m_handler->getRealDimension(x, y, ox, oy, m_batch);
        return;
    }

    if (gameName == "Cube" ||
        gameName == "Untangle"
    )
    {
        return;
    }
    
    if (gameName == "Undead" || gameName == "Dominosa")
    {
        /* For Undead and Dominosa, 
         * the real dimension is calculated by the boundingBox just after removing the outer background */
        genericRemoveSpace();
        getRealDimensionByBoundingBox(x, y, ox, oy);
        return;
    }

    preprocessBatch();
    getRealDimensionByBoundingBox(x, y, ox, oy);
}

void KnotRendererPlasma::getRealDimensionByBoundingBox(int& x, int& y, int& ox, int& oy)
{
    double x1 = x, x2 = 0, y1 = y, y2 = 0;
    
    for (int i = 0; i < m_batch.size(); ++i)
    {
        QRectF bbox;
        
        bbox = m_batch[i]->boundingBox();
        x1 = qMin(x1, floor(bbox.left()));
        x2 = qMax(x2, ceil(bbox.right()));
        y1 = qMin(y1, floor(bbox.top()));
        y2 = qMax(y2, ceil(bbox.bottom()));
    }
    
    if (x1 <= x2)
    {
        x = ceil(x2) - floor(x1);
        y = ceil(y2) - floor(y1);
        ox = floor(x1);
        oy = floor(y1);
    }
}

void KnotRendererPlasma::preprocessBatch()
{
    knotDebugClear("Batch");
    for (auto it = m_batch.begin(); it != m_batch.end(); ++it)
        knotDebugAppend("Batch", (*it)->toString());
    knotDebugFlush();

    if (!m_initialized)
        return; // How could this be?
    if (m_handler)
    {
        m_handler->preprocessBatch(m_batch);
        return;
    }
    genericRemoveSpace();
}

void KnotRendererPlasma::genericRemoveSpace()
{
    if (typeid(**this->m_batch.begin()) == typeid(KnotBatchRectAction))
    {
        this->m_batch.erase(this->m_batch.begin());
    }
}

void KnotRendererPlasma::KnotPlasmaBlockAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    parent->d->fifteen->paint(paint_interface->p, x, y, w, h);
}

void KnotRendererPlasma::KnotPlasmaCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
/*
    if (parent->d->roundFallbackValid)
    {
        qreal left, top, right, bottom;
        parent->d->roundFallback->getMargins(left, top, right, bottom);
        if (radius * 2 >= left + right && radius * 2 >= top + bottom || canGrow)
        {
            qreal realRadius = radius;
            if ((left+right)/2 > realRadius)
                realRadius = (left+right) / 2;
            if ((top+bottom)/2 > realRadius)
                realRadius = (top+bottom) / 2;
            parent->d->roundFallback->resizeFrame(QSizeF(realRadius * 2, realRadius * 2));
            parent->d->roundFallback->paintFrame(paint_interface->p, QPointF(cx-realRadius, cy-realRadius));

            return;
        }
    }
    if (parent->d->roundValid)
    {
        parent->d->round->resizeFrame(QSizeF(radius*2, radius*2));
        parent->d->round->paintFrame(paint_interface->p, QPointF(cx-radius, cy-radius));
        return;
    }
*/
    // fall back to just draw a circle
    // but we don't even know what color to use now
    paint_interface->set(0, 1, 1, color_list);
    paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2));
}

#include "Knotrenderer-plasma.moc"
