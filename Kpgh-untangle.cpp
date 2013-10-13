#include "Knotplasmagamehandler.h"
#include <typeinfo>

UntangleGameHandler::UntangleGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UntangleGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    preprocessBatch(batch);
    for (auto it = batch.begin(); it != batch.end(); ++it)
        if ((*it)->contains(point))
            return true;
    return false;
}
void UntangleGameHandler::free()
{
    delete this;
}
void UntangleGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    return;
}
void UntangleGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     * 
     * Special: do not remove if it's in an unexpected color (which is possible in the victory animation)
     */
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction) &&
        (std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*batch.begin()))->colour == 1
    )
    {
        batch.erase(batch.begin());
    }
}
UntangleGameHandler::~UntangleGameHandler()
{

}
