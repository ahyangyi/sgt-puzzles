#include <QPainter>

#include "Knotrenderer-plasma.h"
#include "Knotdebug.h"
#include "Knotconfig.h"
#include <cmath>
#include <Plasma/Theme>
#include <typeinfo>

class KnotRendererPlasma::Private
{
public:
    static bool isMagical (QColor color)
    {
        return color.red() >= 21 && color.red() <= 25 &&
            color.green() >= 12 && color.green() <= 16 &&
            color.blue() >= 79 && color.blue() <= 83;
    };
    KConfigGroup m_cg;
};

KnotRendererPlasma::KnotRendererPlasma(QGraphicsItem* parent, Qt::WindowFlags wFlags): KnotRendererBatch(parent, wFlags), d(new Private())
{

}

KnotRendererPlasma::~KnotRendererPlasma()
{

}

void KnotRendererPlasma::initialize(KConfigGroup cg)
{
    d->m_cg = cg;
    KnotRenderer::initialize(cg);
}

void KnotRendererPlasma::geometryChangedHandler()
{
    if (!m_initialized)
        return;
    
    int x = int(contentsRect().width()), y = int(contentsRect().height());
    int cx = x, cy = y;
    
    while (true)
    {
        int rx = cx, ry = cy, ox, oy;
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
            
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = NULL;
    
        if (rx > x || ry > y)
            break;
        cx *= 2;
        cy *= 2;
    }
    
    int lx = cx/2, ly = cy/2;
    while (lx < cx || ly < cy)
    {
        int rx = (lx+cx+1)/2, ry = (ly+cy+1)/2, ox, oy;
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
            
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = NULL;

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
        int rx, ry, ox, oy;
        rx = cx;
        ry = cy;
        
        emit sizeRequest(&rx, &ry);

        m_paint_interface = new PaintInterfaceData();
        
        emit forceRedrawRequest();
        
        getRealDimension(rx, ry, ox, oy);
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = NULL;

        setOffset(contentsRect().center() - QPointF(rx / 2., ry / 2.) - QPointF(ox, oy));
    }
}

void KnotRendererPlasma::getRealDimension(int& x, int& y, int &ox, int &oy)
{
    QString gameName = KnotConfig::getGameName(d->m_cg);

    ox = oy = 0;
    
    if (gameName == "Cube")
    {
        return;
    }
    
    getRealDimensionGeneric(x, y, ox, oy);
}

void KnotRendererPlasma::getRealDimensionGeneric(int& x, int& y, int& ox, int& oy)
{
    double x1 = x, x2 = 0, y1 = y, y2 = 0;
    
    preprocessBatch();
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
    else
        ox = oy = 0;
}

void KnotRendererPlasma::preprocessBatch()
{
    knotDebugClear("Batch");
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        knotDebugAppend("Batch", (*it)->toString());
    knotDebugFlush();

    if (!m_initialized)
        return; // How could this be?
    
    QString gameName = KnotConfig::getGameName(d->m_cg);
    if (gameName == "Bridges")
        preprocessBridges();
    if (gameName == "Cube")
        preprocessCube();
    if (gameName == "Fifteen")
        preprocessFifteen();
    if (gameName == "Filling")
        preprocessFilling();
    if (gameName == "Galaxies")
        preprocessGalaxies();
    if (gameName == "Loopy")
        preprocessLoopy();
    if (gameName == "Mines")
        preprocessMines();
    if (gameName == "Signpost")
        preprocessSignpost();
    if (gameName == "Slide")
        preprocessSlide();
}

void KnotRendererPlasma::preprocessBridges()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
    /*
     * Step 2: throw away all the smaller background rectangle with color 0.
     */
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if ((*it)->type() == KNOTBATCH_RECTACTION)
        {
            KnotBatchRectAction *rect = (KnotBatchRectAction *)(*it);
            
            if (rect->colour == 0)
            {
                delete rect;
                it = m_batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}

void KnotRendererPlasma::preprocessCube()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
}

void KnotRendererPlasma::preprocessFifteen()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
}

void KnotRendererPlasma::preprocessFilling()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
}

void KnotRendererPlasma::preprocessGalaxies()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
    /*
     * Step 2: remove the black outer frame.
     */
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());

    /*  
     * Step 3: change any KnotPlasmaCircleAction 
     */
}

void KnotRendererPlasma::preprocessLoopy()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete this->m_batch.at(1);
    this->m_batch.erase(++this->m_batch.begin());
}

void KnotRendererPlasma::preprocessMines()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
}

void KnotRendererPlasma::preprocessSignpost()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
}

void KnotRendererPlasma::preprocessSlide()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
}

void KnotRendererPlasma::KnotPlasmaRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    
}

void KnotRendererPlasma::KnotPlasmaCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    paint_interface->set(0, 0, 0, color_list);
    paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2));
}

#include "Knotrenderer-plasma.moc"
