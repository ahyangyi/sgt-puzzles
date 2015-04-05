#include "Knotplasmagamehandler.h"
#include <limits>

MinesGameHandler::MinesGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool MinesGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void MinesGameHandler::free()
{
    delete this;
}
void MinesGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    DefaultGameHandler::getRealDimension(x, y, ox, oy, batch);
}
void MinesGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove the first 2 polygons and add a frame out of this.
     */
    {
        int count = 0;
        qreal x1 = std::numeric_limits<qreal>::max();
        qreal y1 = std::numeric_limits<qreal>::max();
        qreal x2 = std::numeric_limits<qreal>::min();
        qreal y2 = std::numeric_limits<qreal>::min();

        for (auto it = batch.begin(); it != batch.end();)
        {
            if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchPolyAction))
            {
                auto poly = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchPolyAction>(*it);
                
                for (auto jt = poly->polygon.begin(); jt != poly->polygon.end(); ++jt)
                {
                    if (jt->x() < x1) x1 = jt->x();
                    if (jt->y() < y1) y1 = jt->y();
                    if (jt->x() > x2) x2 = jt->x();
                    if (jt->y() > y2) y2 = jt->y();
                }
                    
                *it = nullptr;
                it = batch.erase(it);
            }
            else
            {
                ++it;
            }
            if (++count == 2)
            {
                break;
            }
        }
        auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.rect_factory->getAction(
            x1, y1, x2 - x1, y2 - y1, true, true, true, true, KnotPlasmaRectAction::SUNKEN));
        
        batch.push_front(neo);
    }
}
MinesGameHandler::~MinesGameHandler()
{

}
