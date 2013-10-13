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
        virtual ~GameHandler() {}
        virtual void preprocessBatch (QList<std::shared_ptr<KnotBatchAction>>& batch) = 0;
        virtual void getRealDimension(int &x, int &y, int &ox, int &oy, QList<std::shared_ptr<KnotBatchAction>>& batch) = 0;
        virtual bool contains (const QPointF& point, const QList<std::shared_ptr<KnotBatchAction>>& batch, const QSizeF& size) = 0;
        virtual void free () = 0;
    };
    class GameHandlerFactory
    {
    public:
        virtual ~GameHandlerFactory() {}
        virtual GameHandler* getGameHandler(const KConfigGroup& cg) = 0;
    };
    KnotRendererPlasma (QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    KnotRendererPlasma (GameHandlerFactory* handlerFactory, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~KnotRendererPlasma();
    virtual void themeChangedHandler ();
    
    virtual bool contains(const QPointF& point) const;
public slots:
    virtual void geometryChangedHandler ();
    virtual void configChangedHandler (KConfigGroup cg);
protected:
    virtual void preprocessBatch(QList<std::shared_ptr<KnotBatchAction>>& m_batch);
    void getRealDimension(int &x, int &y, int &ox, int &oy);
    void getRealDimensionByBoundingBox(int &x, int &y, int &ox, int &oy);
    
    GameHandlerFactory* m_handler_factory;
    GameHandler* m_handler;

private:
    class Private;
    Private *d;
};

#endif
