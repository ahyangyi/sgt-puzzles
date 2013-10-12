#include "Knotplasmagamehandler.h"

SignpostGameHandler::SignpostGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SignpostGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SignpostGameHandler::free()
{
    delete this;
}
void SignpostGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void SignpostGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SignpostGameHandler::~SignpostGameHandler()
{

}
