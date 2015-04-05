#include "Knotplasmagamehandler.h"
#include <typeinfo>

DominosaGameHandler::DominosaGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool DominosaGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void DominosaGameHandler::free()
{
    delete this;
}
void DominosaGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void DominosaGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: throw away all the smaller background rectangle with color 0.
     * 
     * Also, record their coordinates for future use.
     */
    
    QList<int> x1, y1, x2, y2;
    int i = 0, j = 0;

    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            auto rect = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*it);
            
            if (rect->colour == 0)
            {
                if (j > 0 && rect->x == x1[0])
                {
                    i ++;
                    j = 0;
                }
                if (j == x1.size()) x1.push_back(0);
                x1[j] = rect->x;
                if (j == x2.size()) x2.push_back(0);
                x2[j] = rect->x + rect->w;
                if (i == y1.size()) y1.push_back(0);
                y1[i] = rect->y;
                if (i == y2.size()) y2.push_back(0);
                y2[i] = rect->y + rect->h;
                j ++;
                
                it = batch.erase(it);
                continue;
            }
        }
        ++ it;
    }
    
    int n = i + 1, m = j;
    
    /*
     * Step 3: Merge those black round-corner rectangles into our own version of "Dominos"
     * 
     * Also removes the clip actions by the way.
     */

    i = 0; j = 0;
    for (auto it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchClipAction))
        {
            // Erase the clip action, which marks the beginning of a "section"
            it = batch.erase(it);
            
            if (it != batch.end() && typeid(**it) == typeid(KnotRendererBatch::KnotBatchCircleAction))
            {
                auto circle1 = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchCircleAction>(*it);
                auto circle2 = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchCircleAction>(*(it + 1));
                auto rect1 = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*(it + 2));
                auto rect2 = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*(it + 3));

                // We are taking things for granted. So we need some sanity check here to avoid bad things just in case.
                if (!circle1 || !circle2 || !rect1 || !rect2)
                {
                    // do nothing
                }
                else
                {
                    if (circle1->cx == circle2->cx)
                    {
                        // A horizontal domino
                        
                        if (circle1->cx == rect1->x)
                        {
                            // The left part of a domino
                            auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.rect_factory->getAction(
                                circle1->cx-circle1->radius, circle1->cy-circle1->radius,
                                (x2[j]-circle1->cx+circle1->radius)*2, rect1->h, true, true, true, true, KnotPlasmaRectAction::DEFAULT));
                            
                            *it = neo;
                            it ++;
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                        }
                        else
                        {
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                        }
                    }
                    else
                    {
                        // A vertical domino
                        
                        if (circle1->cy == rect2->y)
                        {
                            // The left part of a domino
                            auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.rect_factory->getAction(
                                circle1->cx-circle1->radius, circle1->cy-circle1->radius,
                                rect2->w, (y2[i]-circle1->cy+circle1->radius)*2, true, true, true, true, KnotPlasmaRectAction::DEFAULT));
                            
                            *it = neo;
                            it ++;
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                        }
                        else
                        {
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                            it = batch.erase(it);
                        }
                    }
                }
            }

            j ++;
            if (j == m)
            {
                j = 0;
                i ++;
            }
            
            // Skip until the unclip action
            while (it != batch.end() && typeid(**it) != typeid(KnotRendererBatch::KnotBatchUnclipAction))
                ++it;
            
            if (it != batch.end())
                it = batch.erase(it);
        }
        else
        {
            // Not something interesting us right now.
            ++ it;
        }
    }
    
    /*
     * TODO: Change the rendering method of the numbers.
     */
}
DominosaGameHandler::~DominosaGameHandler()
{

}
