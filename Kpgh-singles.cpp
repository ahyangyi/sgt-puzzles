#include "Knotplasmagamehandler.h"

SinglesGameHandler::SinglesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SinglesGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SinglesGameHandler::free()
{
    delete this;
}
void SinglesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void SinglesGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SinglesGameHandler::~SinglesGameHandler()
{

}
