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
}

void KnotRendererPlasma::configChangedHandler(KConfigGroup cg)
{
    d->m_cg = cg;
    
    if (m_handler != nullptr)
        m_handler->free();
    m_handler = m_handler_factory->getGameHandler(cg);

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
            
        m_batch.clear();
        emit forceRedrawRequest();
        getRealDimension(rx, ry, ox, oy);
    
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
            
        m_batch.clear();
        emit forceRedrawRequest();
        getRealDimension(rx, ry, ox, oy);
    
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
        
        m_batch.clear();
        emit forceRedrawRequest();
        getRealDimension(rx, ry, ox, oy);
    
        delete m_paint_interface;
        m_paint_interface = nullptr;

        setOffset(contentsRect().center() - QPointF(rx / 2., ry / 2.) - QPointF(ox, oy));
    }
}

bool KnotRendererPlasma::contains(const QPointF& point) const
{
    if (m_handler)
    {
        return m_handler->contains(point, m_batch, size());
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
}

void KnotRendererPlasma::preprocessBatch(QList<std::shared_ptr<KnotBatchAction>>& m_batch)
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
}

#include "Knotrenderer-plasma.moc"
