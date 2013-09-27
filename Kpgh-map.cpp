#include "Knotplasmagamehandler.h"
#include <typeinfo>

MapGameHandler::MapGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool MapGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void MapGameHandler::free()
{
    delete this;
}
void MapGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void MapGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: enlarge all major rectangles by a little
     */
    
    for (QList<KnotRendererBatch::KnotBatchAction *>::iterator it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchClipAction))
        {
            ++ it;
            if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
            {
                KnotRendererBatch::KnotBatchRectAction *rect = (KnotRendererBatch::KnotBatchRectAction *)(*it);
                
                rect->x --;
                rect->y --;
                rect->h += 2;
                rect->w += 2;
            }
        }
        ++ it;
    }
}
MapGameHandler::~MapGameHandler()
{

}
