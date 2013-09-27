#include "Knotplasmagamehandler.h"
#include <typeinfo>

FillingGameHandler::FillingGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool FillingGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void FillingGameHandler::free()
{
    delete this;
}
void FillingGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void FillingGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
}
FillingGameHandler::~FillingGameHandler()
{

}
