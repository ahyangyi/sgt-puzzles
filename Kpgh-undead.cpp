#include "Knotplasmagamehandler.h"
#include <typeinfo>

UndeadGameHandler::UndeadGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UndeadGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void UndeadGameHandler::free()
{
    delete this;
}
void UndeadGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void UndeadGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: throw away all the smaller rectangle with color 0 and outside the board.
     */
    
    QRectF board = (*batch.begin())->boundingBox();
    
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour == 0 && !board.contains(rect->rectangle()))
            {
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
UndeadGameHandler::~UndeadGameHandler()
{

}
