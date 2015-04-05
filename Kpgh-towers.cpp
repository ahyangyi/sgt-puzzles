#include "Knotplasmagamehandler.h"

TowersGameHandler::TowersGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool TowersGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void TowersGameHandler::free()
{
    delete this;
}
void TowersGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    // Just use the big box size.
    return;
}
void TowersGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);

    /*
     * Step 2: throw away all the smaller background rectangle with color 0 AND on the boarder.
     */
    
    int x1 = 2147483647, y1 = 2147483647, x2 = -1, y2 = -1;
    for (auto it = batch.begin(); it != batch.end(); ++ it)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);

            x1 = std::min(x1, rect->x);
            y1 = std::min(y1, rect->y);
            x2 = std::max(x2, rect->x + rect->w);
            y2 = std::max(y2, rect->y + rect->h);
        }
    }
    
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour == 0 && (
                        rect->x == x1 || rect->y==y1 || rect->x+rect->w == x2 || rect->y+rect->h == y2
                        ))
            {
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
TowersGameHandler::~TowersGameHandler()
{

}
