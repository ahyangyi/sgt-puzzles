#include "Knotplasmagamehandler.h"
#include <typeinfo>

FillingGameHandler::FillingGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool FillingGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void FillingGameHandler::free()
{
    delete this;
}
void FillingGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void FillingGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
FillingGameHandler::~FillingGameHandler()
{

}
