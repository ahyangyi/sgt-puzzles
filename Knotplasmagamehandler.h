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
    const int cx, cy, radius, style;
    const bool canGrow;
    
    enum StyleHint{
        NORMAL,
    } styleHint;
    
    KnotPlasmaCircleAction(int cx, int cy, int radius, int style, bool canGrow, StyleHint styleHint):
        cx(cx), cy(cy), radius(radius), style(style), canGrow(canGrow), styleHint(styleHint) {}
    virtual ~KnotPlasmaCircleAction ();

    virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
};

class KnotPlasmaCircleActionFactory
{
public:
    virtual ~KnotPlasmaCircleActionFactory ();
    virtual KnotPlasmaCircleAction* getAction (int cx, int cy, int radius, int style, KnotPlasmaCircleAction::StyleHint);
};

class KnotplasmaFrameThemedCircleAction : public KnotPlasmaCircleAction
{
};

class KnotplasmaUnthemedCircleAction : public KnotPlasmaCircleAction
{
};

class DefaultCircleActionFactory : public KnotPlasmaCircleActionFactory
{
public:
    DefaultCircleActionFactory();
    virtual ~DefaultCircleActionFactory ();
};

class KnotPlasmaBlockAction  : public KnotRendererBatch::KnotBatchAction
{
public:
    const int x, y, w, h;
    
    KnotPlasmaBlockAction(int n_x, int n_y, int n_w, int n_h):
        x(n_x), y(n_y), w(n_w), h(n_h) {}

    virtual QString toString () {return QString("plasma-block at %1 %2 %3 %4, style %5, edge %6").arg(x).arg(y).arg(w).arg(h);}
    virtual void apply (PaintInterfaceData* paint_interface, const QList<QColor>& color_list);
    
    virtual QRectF boundingBox() {return QRectF(x, y, w, h);}
    virtual bool contains (const QPointF& point) {return boundingBox().contains(point);}
};

class KnotPlasmaBlockActionFactory
{
public:
    virtual ~KnotPlasmaBlockActionFactory ();
    virtual KnotPlasmaBlockAction* getAction (int x, int y, int w, int h);
};

class KnotplasmaFrameThemedBlockAction : public KnotPlasmaBlockAction
{
};

class KnotplasmaUnthemedBlockAction : public KnotPlasmaBlockAction
{
};

class DefaultBlockActionFactory : public KnotPlasmaBlockActionFactory
{
    DefaultBlockActionFactory ();
    virtual ~DefaultBlockActionFactory ();
};

struct GameHandlerFactories
{
    KnotPlasmaCircleActionFactory *circle_factory;
    KnotPlasmaBlockActionFactory *block_factory;
};

class DefaultGameHandler: public KnotRendererPlasma::GameHandler
{
public:
    virtual ~DefaultGameHandler ();
    DefaultGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
protected:
    virtual void genericRemoveSpace(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimensionByBoundingBox (int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    const GameHandlerFactories m_factories;
};

class BridgesGameHandler: public DefaultGameHandler
{
public:
    virtual ~BridgesGameHandler ();
    BridgesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class CubeGameHandler: public DefaultGameHandler
{
public:
    virtual ~CubeGameHandler ();
    CubeGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class DominosaGameHandler: public DefaultGameHandler
{
public:
    virtual ~DominosaGameHandler ();
    DominosaGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class FifteenGameHandler: public DefaultGameHandler
{
public:
    virtual ~FifteenGameHandler ();
    FifteenGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class FillingGameHandler: public DefaultGameHandler
{
public:
    virtual ~FillingGameHandler ();
    FillingGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class FlipGameHandler: public DefaultGameHandler
{
public:
    virtual ~FlipGameHandler ();
    FlipGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class GalaxiesGameHandler: public DefaultGameHandler
{
public:
    virtual ~GalaxiesGameHandler ();
    GalaxiesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class InertiaGameHandler: public DefaultGameHandler
{
public:
    virtual ~InertiaGameHandler ();
    InertiaGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class LoopyGameHandler: public DefaultGameHandler
{
public:
    virtual ~LoopyGameHandler ();
    LoopyGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class MapGameHandler: public DefaultGameHandler
{
public:
    virtual ~MapGameHandler ();
    MapGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class MinesGameHandler: public DefaultGameHandler
{
public:
    virtual ~MinesGameHandler ();
    MinesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class PearlGameHandler: public DefaultGameHandler
{
public:
    virtual ~PearlGameHandler ();
    PearlGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class RangeGameHandler: public DefaultGameHandler
{
public:
    virtual ~RangeGameHandler ();
    RangeGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SameGameGameHandler: public DefaultGameHandler
{
public:
    virtual ~SameGameGameHandler ();
    SameGameGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SignpostGameHandler: public DefaultGameHandler
{
public:
    virtual ~SignpostGameHandler ();
    SignpostGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SinglesGameHandler: public DefaultGameHandler
{
public:
    virtual ~SinglesGameHandler ();
    SinglesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SixteenGameHandler: public DefaultGameHandler
{
public:
    virtual ~SixteenGameHandler ();
    SixteenGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SlantGameHandler: public DefaultGameHandler
{
public:
    virtual ~SlantGameHandler ();
    SlantGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SlideGameHandler: public DefaultGameHandler
{
public:
    virtual ~SlideGameHandler ();
    SlideGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class SoloGameHandler: public DefaultGameHandler
{
public:
    virtual ~SoloGameHandler ();
    SoloGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class UndeadGameHandler: public DefaultGameHandler
{
public:
    virtual ~UndeadGameHandler ();
    UndeadGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class UnequalGameHandler: public DefaultGameHandler
{
public:
    virtual ~UnequalGameHandler ();
    UnequalGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class UnrulyGameHandler: public DefaultGameHandler
{
public:
    virtual ~UnrulyGameHandler ();
    UnrulyGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

class UntangleGameHandler: public DefaultGameHandler
{
public:
    virtual ~UntangleGameHandler ();
    UntangleGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList< KnotRendererBatch::KnotBatchAction* >& batch);
    virtual bool contains(const QPointF& point, QList< KnotRendererBatch::KnotBatchAction* >& batch, const QSizeF& size);
    virtual void free();
};

#endif