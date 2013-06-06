#include <QPainter>

#include <algorithm>
#include "Knotrenderer-plasma.h"
#include "Knotdebug.h"
#include "Knotconfig.h"
#include <cmath>
#include <Plasma/Theme>
#include <typeinfo>

class KnotRendererPlasma::Private
{
public:
    struct KnotPlasmaRectAction : public KnotBatchAction
    {
        int x, y, w, h, style, edge;
        KnotRendererPlasma* parent;
        
        KnotPlasmaRectAction(int n_x, int n_y, int n_w, int n_h, int n_style, int n_edge, KnotRendererPlasma* n_parent):
            x(n_x), y(n_y), w(n_w), h(n_h), style(n_style), edge(n_edge), parent(n_parent){}

        virtual QString toString () {return QString("plasma-rect at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h).arg(style).arg(edge);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    struct KnotPlasmaBlockAction : public KnotBatchAction
    {
        int x, y, w, h;
        KnotRendererPlasma* parent;
        
        KnotPlasmaBlockAction(int n_x, int n_y, int n_w, int n_h, KnotRendererPlasma* n_parent):
            x(n_x), y(n_y), w(n_w), h(n_h), parent(n_parent){}

        virtual QString toString () {return QString("plasma-block at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
        
        virtual QRectF boundingBox() {return QRectF(x, y, w, h);}
    };

    struct KnotPlasmaCircleAction : public KnotBatchAction
    {
        int cx, cy, radius, style;
        bool canGrow;
        KnotRendererPlasma* parent;
        
        KnotPlasmaCircleAction(int n_cx, int n_cy, int n_radius, int n_style, KnotRendererPlasma* n_parent, bool n_canGrow = false):
            cx(n_cx), cy(n_cy), radius(n_radius), style(n_style), parent(n_parent), canGrow(n_canGrow) {}

        virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
        virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    };

    static bool isMagical (QColor color)
    {
        return color.red() >= 21 && color.red() <= 25 &&
            color.green() >= 12 && color.green() <= 16 &&
            color.blue() >= 79 && color.blue() <= 83;
    };
    KConfigGroup m_cg;
    
    Plasma::Svg *fifteen;
    Plasma::FrameSvg *round;
    bool roundValid;
    Plasma::FrameSvg *roundFallback;
    bool roundFallbackValid;
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
    d->roundValid = d->round->isValid();
    if (!d->roundValid)
    {
        d->roundFallback = new Plasma::FrameSvg(this);
        d->roundFallback->setImagePath("widgets/translucentbackground");
        d->roundFallback->setEnabledBorders(Plasma::FrameSvg::AllBorders);
        d->roundFallbackValid = d->roundFallback->isValid();
    }
    d->fifteen = new Plasma::Svg(this);
    d->fifteen->setImagePath("fifteenPuzzle/blanksquare");
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
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
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
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = nullptr;

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
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        knotDebugAppend("Batch", (*it)->toString());
    knotDebugFlush();

    if (!m_initialized)
        return; // How could this be?
    
    QString gameName = KnotConfig::getGameName(d->m_cg);
    if (gameName == "Bridges")
        preprocessBridges();
    else if (gameName == "Cube")
        preprocessCube();
    else if (gameName == "Dominosa")
        preprocessDominosa();
    else if (gameName == "Fifteen")
        preprocessFifteen();
    else if (gameName == "Filling")
        preprocessFilling();
    else if (gameName == "Flip")
        preprocessFlip();
    else if (gameName == "Galaxies")
        preprocessGalaxies();
    else if (gameName == "Inertia")
        preprocessInertia();
    else if (gameName == "Loopy")
        preprocessLoopy();
    else if (gameName == "Map")
        preprocessMap();
    else if (gameName == "Mines")
        preprocessMines();
    else if (gameName == "Pearl")
        preprocessPearl();
    else if (gameName == "Range")
        preprocessRange();
    else if (gameName == "Same Game")
        preprocessSameGame();
    else if (gameName == "Singles")
        preprocessSingles();
    else if (gameName == "Signpost")
        preprocessSignpost();
    else if (gameName == "Slant")
        preprocessSlant();
    else if (gameName == "Slide")
        preprocessSlide();
    else if (gameName == "Solo")
        preprocessSolo();
    else if (gameName == "Undead")
        preprocessUndead();
    else if (gameName == "Unequal")
        preprocessUnequal();
    else if (gameName == "Unruly")
        preprocessUnruly();
    else if (gameName == "Untangle")
        preprocessUntangle();
    else
    {
        genericRemoveSpace();
    }
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
     * Step 2: throw away all the smaller background rectangle with color 0 or 5.
     */
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if (typeid(**it) == typeid(KnotBatchRectAction))
        {
            KnotBatchRectAction *rect = (KnotBatchRectAction *)(*it);
            
            if (rect->colour == 0 || rect->colour == 5)
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

void KnotRendererPlasma::preprocessFifteen()
{
    /*
     * Step 1: throw away the big background rectangle.
     */

    genericRemoveSpace();
    
    /*
     * Step 2: step the outer bevel
     */
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
    /*
     * Step 3: transform to the fifteen squares.
     */
    QRectF blockRect;
    bool valid = false;

    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if (typeid(**it) == typeid(KnotBatchPolyAction))
        {
            KnotBatchPolyAction* poly = (KnotBatchPolyAction*)*it;
            
            blockRect = poly->boundingBox();
            valid = true;
            
            delete poly;
            it = m_batch.erase(it);
            delete *it;
            it = m_batch.erase(it);
        }
        else if (typeid(**it) == typeid(KnotBatchRectAction))
        {
            KnotBatchRectAction* rect = (KnotBatchRectAction*)*it;
            
            if (valid)
            {
                *it = new Private::KnotPlasmaBlockAction(blockRect.x(), blockRect.y(), blockRect.width(), blockRect.height(), this);
                delete rect;
                
                ++ it;
                
                valid = false;
            }
            else
            {
                delete rect;
                it = m_batch.erase(it);
            }
        }
        else
        {
            ++ it;
        }
    }
}

void KnotRendererPlasma::preprocessFilling()
{
    genericRemoveSpace();
}

void KnotRendererPlasma::preprocessFlip()
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
            Private::KnotPlasmaCircleAction *neo = new Private::KnotPlasmaCircleAction(old->cx, old->cy, old->radius, 0, this, true);
            
            *it = neo;
            delete old;
        }
    }
    
    /*
     * Step 4: 
     */
}

void KnotRendererPlasma::preprocessInertia()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace();
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

void KnotRendererPlasma::preprocessSingles()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
}

void KnotRendererPlasma::preprocessSlant()
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

void KnotRendererPlasma::preprocessUndead()
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    genericRemoveSpace();
    
    /*
     * Step 2: throw away all the smaller rectangle with color 0 and outside the board.
     */
    
    QRectF board = (*m_batch.begin())->boundingBox();
    
    for (QList<KnotBatchAction *>::iterator it = m_batch.begin(); it != m_batch.end();)
    {
        if (typeid(**it) == typeid(KnotBatchRectAction))
        {
            KnotBatchRectAction *rect = (KnotBatchRectAction *)(*it);
            
            if (rect->colour == 0 && !board.contains(rect->rectangle()))
            {
                delete rect;
                it = m_batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}

void KnotRendererPlasma::preprocessUnequal()
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

void KnotRendererPlasma::preprocessUnruly()
{
    /*
     * Step 1: throw away the big background rectangle.
     */

    genericRemoveSpace();
}

void KnotRendererPlasma::Private::KnotPlasmaRectAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
}

void KnotRendererPlasma::Private::KnotPlasmaBlockAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    parent->d->fifteen->paint(paint_interface->p, x, y, w, h);
}

void KnotRendererPlasma::Private::KnotPlasmaCircleAction::apply(KnotRendererBatch::PaintInterfaceData* paint_interface, const QList< QColor >& color_list)
{
    if (parent->d->roundValid)
    {
        parent->d->round->resizeFrame(QSizeF(radius*2, radius*2));
        parent->d->round->paintFrame(paint_interface->p, QPointF(cx-radius, cy-radius));
        return;
    }
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

    // fall back to just draw a circle
    // but we don't even know what color to use now
    paint_interface->set(0, 1, 1, color_list);
    paint_interface->p->drawEllipse(QRectF(cx-radius,cy-radius,radius*2,radius*2));
}

#include "Knotrenderer-plasma.moc"
