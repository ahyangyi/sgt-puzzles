#include "Knotplasmagamehandler.h"
#include <typeinfo>

UntangleGameHandler::UntangleGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UntangleGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
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
void UntangleGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void UntangleGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction) &&
        ((KnotRendererBatch::KnotBatchRectAction*)(*batch.begin()))->colour == 1
    )
    {
        delete *(batch.begin());
        batch.erase(batch.begin());
    }
}
UntangleGameHandler::~UntangleGameHandler()
{

}
