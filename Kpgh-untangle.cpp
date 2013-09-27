#include "Knotplasmagamehandler.h"

UntangleGameHandler::UntangleGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UntangleGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
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
    genericRemoveSpace(batch);
}
UntangleGameHandler::~UntangleGameHandler()
{

}
