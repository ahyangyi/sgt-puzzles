#include "Knotplasmagamehandler.h"

LoopyGameHandler::LoopyGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool LoopyGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void LoopyGameHandler::free()
{
    delete this;
}
void LoopyGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void LoopyGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);

    /*
     * Step 2: throw away the big background rectangle, again.
     */
    batch.erase(batch.begin());
}
LoopyGameHandler::~LoopyGameHandler()
{

}
