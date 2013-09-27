#include "Knotplasmagamehandler.h"
#include <typeinfo>

SixteenGameHandler::SixteenGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool SixteenGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return DefaultGameHandler::contains(point, batch, size);
}
void SixteenGameHandler::free()
{
    delete this;
}
void SixteenGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
    getRealDimensionByBoundingBox(x,y,ox,oy,batch);
}
void SixteenGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    /*
     * Step 1: throw away the big background rectangle.
     */
    genericRemoveSpace(batch);
    
    /*
     * Step 2: remove the outer bevel
     */
    delete *(batch.begin());
    batch.erase(batch.begin());
    delete *(batch.begin());
    batch.erase(batch.begin());
    
    /*
     * Step 3: transform to the fifteen squares.
     */
    QRectF blockRect;
    bool valid = false;

    for (QList<KnotRendererBatch::KnotBatchAction *>::iterator it = batch.begin(); it != batch.end();)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchPolyAction))
        {
            KnotRendererBatch::KnotBatchPolyAction* poly = (KnotRendererBatch::KnotBatchPolyAction*)*it;
            
            blockRect = poly->boundingBox();
            valid = true;
            
            delete poly;
            it = batch.erase(it);
            delete *it;
            it = batch.erase(it);
        }
        else if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchRectAction))
        {
            KnotRendererBatch::KnotBatchRectAction* rect = (KnotRendererBatch::KnotBatchRectAction*)*it;
            
            if (valid)
            {
                // FIXME: Commented for now since we do not have the "this" pointer.
//                *it = new KnotRendererPlasma::KnotPlasmaBlockAction(blockRect.x(), blockRect.y(), blockRect.width(), blockRect.height(), this);
//                delete rect;
                
                ++ it;
                
                valid = false;
            }
            else
            {
                delete rect;
                it = batch.erase(it);
            }
        }
        else
        {
            ++ it;
        }
    }
}
SixteenGameHandler::~SixteenGameHandler()
{

}
