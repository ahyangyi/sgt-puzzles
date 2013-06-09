#ifndef Knotrenderer_plasma_HEADER
#define Knotrenderer_plasma_HEADER

#include "Knotrenderer-batch.h"

struct PaintInterfaceData;

class KnotRendererPlasma: public KnotRendererBatch
{
    Q_OBJECT
public:
    class GameHandler
    {
    public:
        virtual void preprocessBatch (QList<KnotBatchAction*>& batch) = 0;
        virtual void getRealDimension(int &x, int &y, int &ox, int &oy, QList<KnotBatchAction*>& batch) = 0;
        virtual bool contains (const QPointF& point, QList<KnotBatchAction*>& batch) = 0;
    };
    class GameHandlerFactory
    {
    public:
        virtual ~GameHandlerFactory() {}
        virtual GameHandler* getGameHandler(const QString& gameName) = 0;
    };
    KnotRendererPlasma (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    KnotRendererPlasma (GameHandlerFactory* handlerFactory, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererPlasma();
    virtual void themeChangedHandler ();
    
    // Override
    virtual bool contains(const QPointF& point) const;
public slots:
    virtual void initialize (KConfigGroup cg);
    virtual void geometryChangedHandler ();
protected:
    virtual void preprocessBatch();
    void genericRemoveSpace();
    void preprocessBridges();
    void preprocessCube();
    void preprocessDominosa();
    void preprocessFifteen();
    void preprocessFilling();
    void preprocessFlip();
    void preprocessGalaxies();
    void preprocessInertia();
    void preprocessLoopy();
    void preprocessMap();
    void preprocessMines();
    void preprocessPearl();
    void preprocessRange();
    void preprocessSameGame();
    void preprocessSignpost();
    void preprocessSingles();
    void preprocessSixteen();
    void preprocessSlant();
    void preprocessSlide();
    void preprocessSolo();
    void preprocessUnequal();
    void preprocessUndead();
    void preprocessUnruly();
    void preprocessUntangle();
    void getRealDimension(int &x, int &y, int &ox, int &oy);
    void getRealDimensionByBoundingBox(int &x, int &y, int &ox, int &oy);
    
    GameHandlerFactory* m_handler_factory;
    GameHandler* m_handler;
    
private:
    class Private;
    Private *d;
};

#endif
