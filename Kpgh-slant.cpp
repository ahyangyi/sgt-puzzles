#include "Knotplasmagamehandler.h"

SlantGameHandler::SlantGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SlantGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SlantGameHandler::free()
{
    delete this;
}
void SlantGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void SlantGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);

    /*
     * Step 2: remove the outer shadow
     */
    batch.erase(batch.begin());
    
    /*
     * Step 3: throw away all the smaller background rectangle with color 0.
     */
    
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour != 1)
            {
                
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
SlantGameHandler::~SlantGameHandler()
{

}
