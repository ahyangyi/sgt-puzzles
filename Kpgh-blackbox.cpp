#include "Knotplasmagamehandler.h"
#include <typeinfo>

BlackBoxGameHandler::BlackBoxGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool BlackBoxGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void BlackBoxGameHandler::free()
{
    delete this;
}
void BlackBoxGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void BlackBoxGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: throw away all the lines.
     */
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchLineAction))
        {
            it = batch.erase(it);
            continue;
        }
        ++ it;
    }
    
    /*
     * Step 3: throw away all the rectangles (which are the small boxes outside the "blackbox") with color 0.
     * TODO: Change these rectangles to something more helpful
     */
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour == 0)
            {
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
}
BlackBoxGameHandler::~BlackBoxGameHandler()
{

}
