#include "Knotplasmagamehandler.h"

InertiaGameHandler::InertiaGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool InertiaGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void InertiaGameHandler::free()
{
    delete this;
}
void InertiaGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void InertiaGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
InertiaGameHandler::~InertiaGameHandler()
{

}
