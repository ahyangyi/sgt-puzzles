#include "Knotplasmagamehandler.h"

SinglesGameHandler::SinglesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SinglesGameHandler::contains(const QPointF& point, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SinglesGameHandler::free()
{
    delete this;
}
void SinglesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void SinglesGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
SinglesGameHandler::~SinglesGameHandler()
{

}
