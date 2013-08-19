#include "Knotplasmagamehandler.h"
#include "Knotconfig.h"
#include <cmath>
#include <typeinfo>

void DefaultGameHandler::genericRemoveSpace(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction))
    {
        delete *(batch.begin());
        batch.erase(batch.begin());
    }
}

void DefaultGameHandler::getRealDimensionByBoundingBox(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    double x1 = x, x2 = 0, y1 = y, y2 = 0;
    
    for (int i = 0; i < batch.size(); ++i)
    {
        QRectF bbox;
        
        bbox = batch[i]->boundingBox();
        x1 = qMin(x1, floor(bbox.left()));
        x2 = qMax(x2, ceil(bbox.right()));
        y1 = qMin(y1, floor(bbox.top()));
        y2 = qMax(y2, ceil(bbox.bottom()));
    }
    
    if (x1 <= x2)
    {
        x = ceil(x2) - floor(x1);
        y = ceil(y2) - floor(y1);
        ox = floor(x1);
        oy = floor(y1);
    }
}

DefaultGameHandler::~DefaultGameHandler()
{

}

bool DefaultGameHandler::contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    return true;
}

void DefaultGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    this->preprocessBatch(batch);
    this->getRealDimensionByBoundingBox(x, y, ox, oy, batch);
}

void DefaultGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    genericRemoveSpace(batch);
}

void DefaultGameHandler::free()
{
    delete this;
}


class GameHandlerFactoryImpl::Private
{
public:
    static GameHandlerFactoryImpl* instance;
};

GameHandlerFactoryImpl* GameHandlerFactoryImpl::Private::instance = nullptr;

GameHandlerFactoryImpl::GameHandlerFactoryImpl()
{
}

GameHandlerFactoryImpl::~GameHandlerFactoryImpl()
{

}

KnotRendererPlasma::GameHandler* GameHandlerFactoryImpl::getGameHandler(const KConfigGroup& cg)
{
    QString gameName = KnotConfig::getGameName(cg);
    
    if (gameName == "Bridges")
        return new BridgesGameHandler();
    if (gameName == "Cube")
        return new CubeGameHandler();
    if (gameName == "Dominosa")
        return new DominosaGameHandler();
    if (gameName == "Galaxies")
        return new GalaxiesGameHandler();
    
    return new DefaultGameHandler();
}


GameHandlerFactoryImpl* GameHandlerFactoryImpl::instance()
{
    if (Private::instance == nullptr)
        Private::instance = new GameHandlerFactoryImpl();
    return Private::instance;
}

#include "Knotplasmagamehandler.moc"