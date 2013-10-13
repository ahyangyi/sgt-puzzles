#include "Knotplasmagamehandler.h"

SlantGameHandler::SlantGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SlantGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SlantGameHandler::free()
{
    delete this;
}
void SlantGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void SlantGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SlantGameHandler::~SlantGameHandler()
{

}
