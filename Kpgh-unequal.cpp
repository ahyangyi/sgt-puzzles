#include "Knotplasmagamehandler.h"
#include <limits>

UnequalGameHandler::UnequalGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UnequalGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void UnequalGameHandler::free()
{
    delete this;
}
void UnequalGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void UnequalGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove all polygons with size 4, which are frames of the squares, which will be better represented with plasma frames.
     */
    for (auto it = batch.begin(); it != batch.end(); ++ it)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchPolyAction))
        {
            auto poly = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchPolyAction>(*it);
            
            if (poly->polygon.size() == 4)
            {
                qreal x1 = std::numeric_limits<qreal>::max();
                qreal y1 = std::numeric_limits<qreal>::max();
                qreal x2 = std::numeric_limits<qreal>::min();
                qreal y2 = std::numeric_limits<qreal>::min();

                for (auto it = poly->polygon.begin(); it != poly->polygon.end(); ++ it)
                {
                    if (it->x() < x1) x1 = it->x();
                    if (it->y() < y1) y1 = it->y();
                    if (it->x() > x2) x2 = it->x();
                    if (it->y() > y2) y2 = it->y();
                }
                auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.rect_factory->getAction(
                    x1, y1, x2 - x1, y2 - y1, true, true, true, true, KnotPlasmaRectAction::SUNKEN));
                
                *it = neo;
            }
        }
    }

    /*
     * Step 3: remove all rectangles.
     */

    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            it = batch.erase(it);
            continue;
        }
        ++ it;
    }
}
UnequalGameHandler::~UnequalGameHandler()
{

}
