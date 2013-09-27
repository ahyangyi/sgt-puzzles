#include "Knotplasmagamehandler.h"

SoloGameHandler::SoloGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SoloGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SoloGameHandler::free()
{
    delete this;
}
void SoloGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void SoloGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SoloGameHandler::~SoloGameHandler()
{

}
