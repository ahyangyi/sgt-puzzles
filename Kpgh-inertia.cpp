#include "Knotplasmagamehandler.h"
#include <typeinfo>

InertiaGameHandler::InertiaGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool InertiaGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void InertiaGameHandler::free()
{
    delete this;
}
void InertiaGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void InertiaGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
InertiaGameHandler::~InertiaGameHandler()
{

}
