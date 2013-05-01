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
    Plasma::FrameSvg *round;
};

KnotRendererPlasma::KnotRendererPlasma(QGraphicsItem* parent, Qt::WindowFlags wFlags): KnotRendererBatch(parent, wFlags), d(new Private())
{
}

KnotRendererPlasma::~KnotRendererPlasma()
{

}

void KnotRendererPlasma::themeChangedHandler()
{
    KnotRendererBatch::themeChangedHandler();
}

void KnotRendererPlasma::initialize(KConfigGroup cg)
{
    d->m_cg = cg;
    d->round = new Plasma::FrameSvg(this);
    d->round->setImagePath("widgets/circular-background");
    d->round->setEnabledBorders(Plasma::FrameSvg::NoBorder);
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
        int rx = cx, ry = cy, ox = 0, oy = 0;
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
        int rx = (lx+cx+1)/2, ry = (ly+cy+1)/2, ox = 0, oy = 0;
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
        int rx = cx, ry = cy, ox = 0, oy = 0;
        
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

    if (gameName == "Cube" ||
        gameName == "Untangle"
    )
    {
        return;
    }
    
    if (gameName == "Dominosa")
    {
        getRealDimensionDominosa(x, y, ox, oy);
    }
    
    getRealDimensionGeneric(x, y, ox, oy);
}

void KnotRendererPlasma::getRealDimensionDominosa(int& x, int& y, int& ox, int& oy)
{
    double x1 = x, x2 = 0, y1 = y, y2 = 0;
    
    for (int i = 1; i < m_batch.size(); ++i)
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
    if (gameName == "Dominosa")
        preprocessDominosa();
    if (gameName == "Fifteen")
        preprocessFifteen();
    if (gameName == "Filling")
        preprocessFilling();
    if (gameName == "Galaxies")
        preprocessGalaxies();
    if (gameName == "Loopy")
        preprocessLoopy();
    if (gameName == "Map")
        preprocessMap();
    if (gameName == "Mines")
        preprocessMines();
    if (gameName == "Pearl")
        preprocessPearl();
    if (gameName == "Range")
        preprocessRange();
    if (gameName == "Same Game")
        preprocessSameGame();
    if (gameName == "Signpost")
        preprocessSignpost();
    if (gameName == "Slide")
        preprocessSlide();
    if (gameName == "Solo")
        preprocessSolo();
    if (gameName == "Untangle")
        preprocessUntangle();
}

void KnotRendererPlasma::genericRemoveSpace()
{
    if (typeid(**this->m_batch.begin()) == typeid(KnotBatchRectAction))
    {
        delete *(this->m_batch.begin());
        this->m_batch.erase(this->m_batch.begin());
    }
}

void KnotRendererPlasma::preprocessBridges()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
    /*
     * Step 2: throw away all the smaller background rectangle with color 0.
     */
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if (typeid(**it) == typeid(KnotBatchRectAction))
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
    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessDominosa()
{
    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessFifteen()
{
    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessFilling()
{
    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessGalaxies()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace();
    
    /*
     * Step 2: remove the black outer frame.
     */
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());

    /*  
     * Step 3: change any KnotPlasmaCircleAction 
     */
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
    {
        if (typeid(**it) == typeid(KnotBatchCircleAction))
        {
            KnotBatchCircleAction *old = (KnotBatchCircleAction *)(*it);
            KnotPlasmaCircleAction *neo = new KnotPlasmaCircleAction(old->cx, old->cy, old->radius, 0, this);
            
            *it = neo;
            delete old;
        }
    }
    
    /*
     * Step 4: 
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

void KnotRendererPlasma::preprocessMap()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
    /*
     * Step 2: enlarge all major rectangles by a little
     */
    
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if (typeid(**it) == typeid(KnotBatchClipAction))
        {
            ++ it;
            if (typeid(**it) == typeid(KnotBatchRectAction))
            {
                KnotBatchRectAction *rect = (KnotBatchRectAction *)(*it);
                
                rect->x --;
                rect->y --;
                rect->h += 2;
                rect->w += 2;
            }
        }
        ++ it;
    }
}

void KnotRendererPlasma::preprocessMines()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessPearl()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessRange()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessSameGame()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessSignpost()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessSlide()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessSolo()
{
    /*
     * Step 1: throw away the big background rectangle.
     */

    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessUntangle()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::KnotPlasmaRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
}

void KnotRendererPlasma::KnotPlasmaCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    parent->d->round->resizeFrame(QSizeF(radius*2, radius*2));
    parent->d->round->paintFrame(paint_interface->p, QPointF(cx-radius, cy-radius));
}

#include "Knotrenderer-plasma.moc"
