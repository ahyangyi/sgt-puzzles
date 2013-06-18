#ifndef Knotplasmagamehandler_HEADER
#define Knotplasmagamehandler_HEADER

#include "Knotrenderer-plasma.h"

class GameHandlerFactoryImpl: public KnotRendererPlasma::GameHandlerFactory
{
public:
    virtual ~GameHandlerFactoryImpl();
    virtual KnotRendererPlasma::GameHandler* getGameHandler(const KConfigGroup& cg);
    static GameHandlerFactoryImpl* instance ();
protected: 
    GameHandlerFactoryImpl ();
private:
    class Private;
    Private* d;
};

class KnotPlasmaCircleAction : public KnotRendererBatch::KnotBatchAction
{
public:
    int cx, cy, radius, style;
    bool canGrow;
    
    enum StyleHint{
        NORMAL,
    } styleHint;
    
    KnotPlasmaCircleAction(int n_cx, int n_cy, int n_radius, int n_style, StyleHint n_styleHint):
        cx(n_cx), cy(n_cy), radius(n_radius), style(n_style), styleHint(n_styleHint) {}
    virtual ~KnotPlasmaCircleAction ();

    virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
    virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class KnotPlasmaCircleActionFactory
{
public:
    ~KnotPlasmaCircleActionFactory ();
    KnotPlasmaCircleAction* getAction (int cx, int cy, int radius, int style, KnotPlasmaCircleAction::StyleHint);
};

class DefaultGameHandler: public KnotRendererPlasma::GameHandler
{
public:
    virtual ~DefaultGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void free();
protected:
    virtual void genericRemoveSpace(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimensionByBoundingBox (int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
};

class BridgesGameHandler: public DefaultGameHandler
{
public:
    virtual ~BridgesGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void free();
};

class CubeGameHandler: public DefaultGameHandler
{
public:
    virtual ~CubeGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void free();
};

class DominosaGameHandler: public DefaultGameHandler
{
public:
    virtual ~DominosaGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void free();
};

class GalaxiesGameHandler: public DefaultGameHandler
{
public:
    virtual ~GalaxiesGameHandler ();
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, const QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void free();
};

#endif