#include "Knotplasmagamehandler.h"

UnequalGameHandler::UnequalGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UnequalGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void UnequalGameHandler::free()
{
    delete this;
}
void UnequalGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void UnequalGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
UnequalGameHandler::~UnequalGameHandler()
{

}
