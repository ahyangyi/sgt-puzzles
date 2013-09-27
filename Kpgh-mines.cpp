#include "Knotplasmagamehandler.h"

MinesGameHandler::MinesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool MinesGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void MinesGameHandler::free()
{
    delete this;
}
void MinesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void MinesGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
MinesGameHandler::~MinesGameHandler()
{

}
