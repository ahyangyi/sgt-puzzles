#include "Knotplasmagamehandler.h"

UnrulyGameHandler::UnrulyGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UnrulyGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void UnrulyGameHandler::free()
{
    delete this;
}
void UnrulyGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void UnrulyGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
UnrulyGameHandler::~UnrulyGameHandler()
{

}
