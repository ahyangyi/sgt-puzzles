#ifndef Knotplasmagamehandler_HEADER
#define Knotplasmagamehandler_HEADER

#include "Knotrenderer-plasma.h"

class GameHandlerFactoryImpl: public KnotRendererPlasma::GameHandlerFactory
{
public:
    virtual ~GameHandlerFactoryImpl();
    virtual KnotRendererPlasma::GameHandler* getGameHandler(const QString& gameName);
    static GameHandlerFactoryImpl* instance ();
protected: 
    GameHandlerFactoryImpl ();
private:
    class Private;
    Private* d;
};

class DefaultGameHandler: public KnotRendererPlasma::GameHandler
{
public:
    virtual ~DefaultGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch);
};

#endif