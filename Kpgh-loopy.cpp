#include "Knotplasmagamehandler.h"

LoopyGameHandler::LoopyGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool LoopyGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void LoopyGameHandler::free()
{
    delete this;
}
void LoopyGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void LoopyGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     * 
     * Note that GenericRemoveBackground does not work here.
     */
    batch.erase(batch.begin());

    /*
     * Step 2: throw away the big background rectangle, again.
     */
    batch.erase(batch.begin());
}
LoopyGameHandler::~LoopyGameHandler()
{

}
