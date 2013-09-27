#include "Knotplasmagamehandler.h"
#include <typeinfo>

GalaxiesGameHandler::GalaxiesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool GalaxiesGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size  )
{
    return DefaultGameHandler::contains(point, batch, size);
}
void GalaxiesGameHandler::free()
{
    delete this;
}
void GalaxiesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void GalaxiesGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove the black outer frame.
     */
    delete *(batch.begin());
    batch.erase(batch.begin());

    /*  
     * Step 3: change any KnotPlasmaCircleAction 
     */
    for (auto it = batch.begin(); it != batch.end(); ++it)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchCircleAction))
        {
            auto old = (KnotRendererBatch::KnotBatchCircleAction *)(*it);
            auto neo = new KnotRendererPlasma::KnotPlasmaCircleAction(old->cx, old->cy, old->radius, 0, true);
            
            *it = neo;
            delete old;
        }
    }
    
    /*
     * Step 4: 
     */
    
}
GalaxiesGameHandler::~GalaxiesGameHandler()
{

}
