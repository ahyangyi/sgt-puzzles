#include "Knotplasmagamehandler.h"
#include "Knotconfig.h"
#include <cmath>
#include <typeinfo>

DefaultGameHandler::DefaultGameHandler(const GameHandlerFactories& factories): m_factories(factories)
{
}

void DefaultGameHandler::genericRemoveSpace(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    if (typeid(**batch.begin()) == typeid(KnotRendererBatch::KnotBatchRectAction) &&
        ((KnotRendererBatch::KnotBatchRectAction*)(*batch.begin()))->colour == 1
    )
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

bool DefaultGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size)
{
    return (QRectF(QPointF(0,0), size).contains(point));
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
    GameHandlerFactories factories;
    
    if (gameName == "Bridges")
        return new BridgesGameHandler(factories);
    if (gameName == "Cube")
        return new CubeGameHandler(factories);
    if (gameName == "Dominosa")
        return new DominosaGameHandler(factories);
    if (gameName == "Fifteen")
        return new FifteenGameHandler(factories);
    if (gameName == "Filling")
        return new FillingGameHandler(factories);
    if (gameName == "Flip")
        return new FlipGameHandler(factories);
    if (gameName == "Galaxies")
        return new GalaxiesGameHandler(factories);
    if (gameName == "Inertia")
        return new InertiaGameHandler(factories);
    if (gameName == "Loopy")
        return new LoopyGameHandler(factories);
    if (gameName == "Map")
        return new MapGameHandler(factories);
    if (gameName == "Mines")
        return new MinesGameHandler(factories);
    if (gameName == "Pearl")
        return new PearlGameHandler(factories);
    if (gameName == "Range")
        return new RangeGameHandler(factories);
    if (gameName == "Same Game")
        return new SameGameGameHandler(factories);
    if (gameName == "Signpost")
        return new SignpostGameHandler(factories);
    if (gameName == "Singles")
        return new SinglesGameHandler(factories);
    if (gameName == "Sixteen")
        return new SixteenGameHandler(factories);
    if (gameName == "Slant")
        return new SlantGameHandler(factories);
    if (gameName == "Slide")
        return new SlideGameHandler(factories);
    if (gameName == "Solo")
        return new SoloGameHandler(factories);
    if (gameName == "Undead")
        return new UndeadGameHandler(factories);
    if (gameName == "Unequal")
        return new UnequalGameHandler(factories);
    if (gameName == "Unruly")
        return new UnrulyGameHandler(factories);
    if (gameName == "Untangle")
        return new UntangleGameHandler(factories);
    
    return new DefaultGameHandler(factories);
}


GameHandlerFactoryImpl* GameHandlerFactoryImpl::instance()
{
    if (Private::instance == nullptr)
        Private::instance = new GameHandlerFactoryImpl();
    return Private::instance;
}

#include "Knotplasmagamehandler.moc"