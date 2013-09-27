#include "Knotplasmagamehandler.h"

RangeGameHandler::RangeGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool RangeGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void RangeGameHandler::free()
{
    delete this;
}
void RangeGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void RangeGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
RangeGameHandler::~RangeGameHandler()
{

}
