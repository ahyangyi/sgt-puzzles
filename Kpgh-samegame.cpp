#include "Knotplasmagamehandler.h"

SameGameGameHandler::SameGameGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SameGameGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SameGameGameHandler::free()
{
    delete this;
}
void SameGameGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void SameGameGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SameGameGameHandler::~SameGameGameHandler()
{

}
