#include "Knotplasmagamehandler.h"

SlideGameHandler::SlideGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SlideGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SlideGameHandler::free()
{
    delete this;
}
void SlideGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void SlideGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SlideGameHandler::~SlideGameHandler()
{

}
