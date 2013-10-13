#include "Knotplasmagamehandler.h"

UnequalGameHandler::UnequalGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UnequalGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void UnequalGameHandler::free()
{
    delete this;
}
void UnequalGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void UnequalGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove all polygons with size 4, which are frames of the squares, which will be better represented with plasma frames.
     */
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchPolyAction))
        {
            auto poly = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchPolyAction>(*it);
            
            if (poly->polygon.size() == 4)
            {
                it = batch.erase(it);
            }
            else
                ++ it;
        }
        else
            ++ it;
    }
}
UnequalGameHandler::~UnequalGameHandler()
{

}
