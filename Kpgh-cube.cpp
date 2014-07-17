#include "Knotplasmagamehandler.h"
#include <typeinfo>

CubeGameHandler::CubeGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool CubeGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return containsByPreprocessedElements(point, batch, size);
}
void CubeGameHandler::free()
{
    delete this;
}
void CubeGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    return;
}
void CubeGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
CubeGameHandler::~CubeGameHandler()
{

}
