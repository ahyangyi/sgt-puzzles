#include "Knotplasmagamehandler.h"
#include <typeinfo>

FifteenGameHandler::FifteenGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool FifteenGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void FifteenGameHandler::free()
{
    delete this;
}
void FifteenGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void FifteenGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */

    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove the outer bevel
     */
    batch.erase(batch.begin());
    batch.erase(batch.begin());
    
    /*
     * Step 3: transform to the fifteen squares.
     */
    QRectF blockRect;
    bool valid = false;

    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchPolyAction))
        {
            auto poly = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchPolyAction>(*it);
            
            blockRect = poly->boundingBox();
            valid = true;
            
            it = batch.erase(it);
            it = batch.erase(it);
        }
        else if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (valid)
            {
                *it = std::shared_ptr<KnotRendererBatch::KnotBatchAction>(m_factories.block_factory->getAction(blockRect.x(), blockRect.y(), blockRect.width(), blockRect.height()));
                ++ it;
                
                valid = false;
            }
            else
            {
                it = batch.erase(it);
            }
        }
        else
        {
            ++ it;
        }
    }
}
FifteenGameHandler::~FifteenGameHandler()
{

}
