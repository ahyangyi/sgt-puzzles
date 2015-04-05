#include "Knotplasmagamehandler.h"
#include <typeinfo>

GalaxiesGameHandler::GalaxiesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool GalaxiesGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void GalaxiesGameHandler::free()
{
    delete this;
}
void GalaxiesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void GalaxiesGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove the black outer frame.
     */
    batch.erase(batch.begin());

    /*  
     * Step 3: change any KnotPlasmaCircleAction 
     */
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchCircleAction))
        {
            auto old = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchCircleAction>(*it);
            auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.circle_factory->getAction(old->cx, old->cy, old->radius, 0, true, KnotPlasmaCircleAction::DEFAULT));
            
            it = batch.erase(it);
            batch.push_back(neo);
        }
        else
        {
            ++it;
        }
    }
    
    /*
     * Step 4: 
     */
    
}
GalaxiesGameHandler::~GalaxiesGameHandler()
{

}
