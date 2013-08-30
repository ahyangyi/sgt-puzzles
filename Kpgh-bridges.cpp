#include "Knotplasmagamehandler.h"
#include <typeinfo>

bool BridgesGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void BridgesGameHandler::free()
{
    delete this;
}
void BridgesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void BridgesGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: throw away all the smaller background rectangle with color 0 or 5.
     */
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            KnotRendererBatch::KnotBatchRectAction *rect = (KnotRendererBatch::KnotBatchRectAction *)(*it);
            
            if (rect->colour == 0 || rect->colour == 5)
            {
                delete rect;
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
BridgesGameHandler::~BridgesGameHandler()
{

}
