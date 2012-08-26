#include <QPainter>

#include "Knotrenderer-plasma.h"
#include "Knotdebug.h"
#include <Plasma/Theme>

class KnotRendererPlasma::Private
{
public:
    static bool isMagical (QColor color)
    {
        return color.red() >= 21 && color.red() <= 25 &&
            color.green() >= 12 && color.green() <= 16 &&
            color.blue() >= 79 && color.blue() <= 83;
    };
};

KnotRendererPlasma::KnotRendererPlasma(QGraphicsItem* parent, Qt::WindowFlags wFlags): KnotRendererBatch(parent, wFlags)
{

}

KnotRendererPlasma::~KnotRendererPlasma()
{

}

void KnotRendererPlasma::preprocessBatch(QList<QColor> colorList)
{
    knotDebugClear();
    for (QList<KnotBatchAction*>::iterator it = m_batch.begin(); it != m_batch.end(); ++it)
        knotDebugAppend((*it)->toString());
    knotDebugFlush();

    preprocessGalaxies(colorList);
}

void KnotRendererPlasma::preprocessGalaxies(QList< QColor > colorList)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    
    if (this->m_batch.size() == 0)
        return;
    delete *this->m_batch.begin();
    this->m_batch.erase(this->m_batch.begin());
    
    /*
     * Step 2: change any KnotPlasmaCircleAction 
     */
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
