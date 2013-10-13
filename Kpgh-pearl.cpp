#include "Knotplasmagamehandler.h"

PearlGameHandler::PearlGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool PearlGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void PearlGameHandler::free()
{
    delete this;
}
void PearlGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void PearlGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
PearlGameHandler::~PearlGameHandler()
{

}
