#include "Knotplasmagamehandler.h"

DefaultGameHandler::~DefaultGameHandler()
{

}

bool DefaultGameHandler::contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{

}

void DefaultGameHandler::getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch)
{
    this->preprocessBatch(batch);
}

void DefaultGameHandler::preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch)
{

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

KnotRendererPlasma::GameHandler* GameHandlerFactoryImpl::getGameHandler(const QString& gameName)
{
    return new DefaultGameHandler();

}


GameHandlerFactoryImpl* GameHandlerFactoryImpl::instance()
{
    if (Private::instance == nullptr)
        Private::instance = new GameHandlerFactoryImpl();
    return Private::instance;
}
