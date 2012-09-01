#include <QPainter>

#include "Knotrenderer-plasma.h"
#include "Knotdebug.h"
#include "Knotconfig.h"
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
    int tx, ty;
    int rx, ry, ox, oy;
    
    while (true)
    {
        tx = cx; ty = cy;
        emit sizeRequest(&tx, &ty);
        {
            // Fake an interface
            m_paint_interface = new PaintInterfaceData();
            
            emit forceRedrawRequest();
            
            rx = tx;
            ry = ty;
            getRealDimension(rx, ry, ox, oy);
            
            for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
                delete (*it);
            m_batch.clear();
        
            delete m_paint_interface;
            m_paint_interface = NULL;
            
            if (rx > x || ry > y)
                break;
        }
        cx ++;
        cy ++;
    }
    
    cx --;
    cy --;
    tx = cx;
    ty = cy;
    emit sizeRequest(&tx, &ty);
    {
        // Fake an interface
        m_paint_interface = new PaintInterfaceData();
        
        emit forceRedrawRequest();
        
        rx = tx;
        ry = ty;
        getRealDimension(rx, ry, ox, oy);
        
        for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
            delete (*it);
        m_batch.clear();
    
        delete m_paint_interface;
        m_paint_interface = NULL;
    }
    
    setOffset(contentsRect().center() - QPointF(rx / 2., ry / 2.) - QPointF(ox, oy));
}

void KnotRendererPlasma::getRealDimension(int& x, int& y, int &ox, int &oy)
{
    QString gameName = KnotConfig::getGameName(d->m_cg);
    
    if (gameName == "Galaxies")
    {
        getRealDimensionAllRectButFirst(x, y, ox, oy, 2);
        return;
    }
    if (gameName == "Mines" || 
        gameName == "Signpost" ||
        gameName == "Slide"
    )
    {
        getRealDimensionAllRectButFirst(x, y, ox, oy, 1);
        return;
    }
    ox = oy = 0;
}

void KnotRendererPlasma::getRealDimensionSecondRect(int& x, int& y, int &ox, int &oy)
{
    if (m_batch.size() >= 1)
    {
        x = ((KnotPlasmaRectAction*)m_batch[1])->w;
        y = ((KnotPlasmaRectAction*)m_batch[1])->h;
        ox = ((KnotPlasmaRectAction*)m_batch[1])->x;
        oy = ((KnotPlasmaRectAction*)m_batch[1])->y;
    }
    else
    {
        ox = oy = 0;
    }
}

void KnotRendererPlasma::getRealDimensionAllRectButFirst(int& x, int& y, int& ox, int& oy, int n)
{
    if (m_batch.size() >= 1)
    {
        int x1 = x, x2 = 0, y1 = y, y2 = 0;
        for (int i = n; i < m_batch.size(); ++i)
        {
            if (m_batch[i]->type() == KNOTBATCH_RECTACTION)
            {
                KnotPlasmaRectAction *rect = (KnotPlasmaRectAction*)m_batch[i];
                
                x1 = qMin(x1, rect->x);
                x2 = qMax(x2, rect->x + rect->w);
                y1 = qMin(y1, rect->y);
                y2 = qMax(y2, rect->y + rect->h);
            }
        }
        
        knotDebugAppend("getRealDimensionAllRectButFirst", QString("%1 %2 %3 %4").arg(x1).arg(y1).arg(x2).arg(y2));
        knotDebugAppend("getRealDimensionAllRectButFirst", QString("%1 %2").arg(typeid(m_batch.at(1)).name()).arg(typeid(new KnotPlasmaRectAction(0,0,0,0,0,0)).name()));
        knotDebugFlush();
        
        if (x1 <= x2)
        {
            x = x2 - x1;
            y = y2 - y1;
            ox = x1;
            oy = y1;
        }
        else
            ox = oy = 0;
    }
    else
    {
        ox = oy = 0;
    }
}

void KnotRendererPlasma::preprocessBatch(QList<QColor> colorList)
{
    knotDebugClear("Batch");
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        knotDebugAppend("Batch", (*it)->toString());
    knotDebugFlush();

    if (!m_initialized)
        return; // How could this be?
    
    QString gameName = KnotConfig::getGameName(d->m_cg);
    if (gameName == "Galaxies")
        preprocessGalaxies(colorList);
    if (gameName == "Mines")
        preprocessMines(colorList);
    if (gameName == "Signpost")
        preprocessSignpost(colorList);
    if (gameName == "Slide")
        preprocessSlide(colorList);
}

void KnotRendererPlasma::preprocessGalaxies(QList< QColor > colorList)
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

void KnotRendererPlasma::preprocessMines(QList< QColor > colorList)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
}

void KnotRendererPlasma::preprocessSignpost(QList< QColor > colorList)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    delete *(this->m_batch.begin());
    this->m_batch.erase(this->m_batch.begin());
    
}

void KnotRendererPlasma::preprocessSlide(QList< QColor > colorList)
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
