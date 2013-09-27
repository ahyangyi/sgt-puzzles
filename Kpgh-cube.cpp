#include "Knotplasmagamehandler.h"
#include <typeinfo>

CubeGameHandler::CubeGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool CubeGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void CubeGameHandler::free()
{
    delete this;
}
void CubeGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    return;
}
void CubeGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
CubeGameHandler::~CubeGameHandler()
{

}
