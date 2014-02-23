#include "Knotplasmagamehandler.h"
#include <typeinfo>

UntangleGameHandler::UntangleGameHandler(const GameHandlerFactories& factories): DefaultGameHandler(factories)
{
}

bool UntangleGameHandler::contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size)
{
    return containsByPreprocessedElements(point, batch, size);
}
void UntangleGameHandler::free()
{
    delete this;
}
void UntangleGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    return;
}
void UntangleGameHandler::preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch)
{
    /*
     * Color information:
     * 
     *      Circles normally have 4/3 color, 5/3 when clicked, and 6/3 when its neighbor is clicked.
     *      Background rectangle has color 1, unless you win the game, in which case it alternates between 7 and 8.
     */
    /*
     * Step 1: throw away the big background rectangle.
     * 
     * Special: do not remove if it's in an unexpected color (which is possible in the victory animation)
     */
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction))
    {
        auto old = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchRectAction>(*batch.begin());
        static Colorizer c7 = {QColor(0,0,0), 0.2}, c8 = {QColor(255,255,255), 0.2};
        Colorizer *pc;
        
        if (old->colour == 1)
        {
            // Not winning, no colorizer
            pc = nullptr;
        }
        else 
            // winning, colorize the background between black(7) and white(8)
        if (old->colour == 7)
        {
            pc = &c7;
        }
        else
        {
            pc = &c8;
        }
        auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.rect_factory->getAction(
            old->x, old->y, old->w, old->h, true, true, true, true, KnotPlasmaRectAction::SUNKEN, pc));
        
        *batch.begin() = neo;
    }

    /*  
     * Step 2: change any KnotPlasmaCircleAction 
     */
    for (auto it = batch.begin(); it != batch.end(); ++it)
    {
        if (typeid(**it) == typeid(KnotRendererBatch::KnotBatchCircleAction))
        {
            auto old = std::dynamic_pointer_cast<KnotRendererBatch::KnotBatchCircleAction>(*it);
            KnotPlasmaCircleAction::StyleHint style = old->fillColour == 4? KnotPlasmaCircleAction::RAISED:
                old->fillColour == 5? KnotPlasmaCircleAction::SUNKEN:
                KnotPlasmaCircleAction::SUNKEN;
            
            auto neo = std::shared_ptr<KnotRendererPlasma::KnotBatchAction>(m_factories.circle_factory->getAction(
                old->cx, old->cy, old->radius, 0, true, style));
            
            *it = neo;
        }
    }
}
UntangleGameHandler::~UntangleGameHandler()
{

}
