#include "Knotplasmagamehandler.h"
#include <typeinfo>

FlipGameHandler::FlipGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool FlipGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void FlipGameHandler::free()
{
    delete this;
}
void FlipGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void FlipGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
FlipGameHandler::~FlipGameHandler()
{

}
