#include "Knotplasmagamehandler.h"
#include <typeinfo>

DominosaGameHandler::DominosaGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool DominosaGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void DominosaGameHandler::free()
{
    delete this;
}
void DominosaGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void DominosaGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: throw away all the smaller background rectangle with color 0.
     */
    
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour == 0)
            {
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
DominosaGameHandler::~DominosaGameHandler()
{

}
