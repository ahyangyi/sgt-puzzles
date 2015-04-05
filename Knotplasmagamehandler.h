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

struct Colorizer
{
    QColor color;
    qreal strength;
};

class KnotPlasmaCircleAction : public KnotRendererBatch::KnotBatchAction
{
public:
    const int cx, cy, radius, style;
    const bool canGrow;
    
    enum StyleHint{
        DEFAULT, TRANSLUCENT, PLAIN, SUNKEN, RAISED, 
    } styleHint;
    
    KnotPlasmaCircleAction(int cx, int cy, int radius, int style, bool canGrow, StyleHint styleHint):
        cx(cx), cy(cy), radius(radius), style(style), canGrow(canGrow), styleHint(styleHint) {}
    virtual ~KnotPlasmaCircleAction ();

    virtual QString toString () {return QString("plasma-circle at %1 %2 radius %3, style %4").arg(cx).arg(cy).arg(radius).arg(style);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list) = 0;
};

class KnotPlasmaCircleActionFactory
{
public:
    virtual ~KnotPlasmaCircleActionFactory ();
    virtual KnotPlasmaCircleAction* getAction (int cx, int cy, int radius, int style, bool canGrow, KnotPlasmaCircleAction::StyleHint styleHint) = 0;
};

class KnotPlasmaRectAction  : public KnotRendererBatch::KnotBatchAction
{
public:
    const int x, y, w, h;
    const bool t, b, l, r;

    enum StyleHint{
        DEFAULT, TRANSLUCENT, PLAIN, SUNKEN, RAISED, 
    } styleHint;
    
    Colorizer* colorizer;
    
    KnotPlasmaRectAction(int n_x, int n_y, int n_w, int n_h, bool n_t, bool n_b, bool n_l, bool n_r, StyleHint n_styleHint, Colorizer* n_colorizer):
        x(n_x), y(n_y), w(n_w), h(n_h), t(n_t), b(n_b), l(n_l), r(n_l), styleHint(n_styleHint), colorizer(n_colorizer) {}

    virtual QString toString () {return QString("plasma-rect at %1 %2 %3 %4, style %5, edge %6/%7/%8/%9").arg(x).arg(y).arg(w).arg(h).arg((int)styleHint).arg(t).arg(b).arg(l).arg(r);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list) = 0;
    
    virtual QRectF boundingBox() {return QRectF(x, y, w, h);}
    virtual bool contains (const QPointF& point) {return boundingBox().contains(point);}
};

class KnotPlasmaRectActionFactory
{
public:
    virtual ~KnotPlasmaRectActionFactory ();
    virtual KnotPlasmaRectAction* getAction (int x, int y, int w, int h, 
                                             bool t = true, bool b = true, bool l = true, bool r = true, 
                                             KnotPlasmaRectAction::StyleHint styleHint = KnotPlasmaRectAction::DEFAULT, Colorizer* colorizer = nullptr) = 0;
};

class KnotPlasmaBlockAction  : public KnotRendererBatch::KnotBatchAction
{
public:
    const int x, y, w, h;
    
    KnotPlasmaBlockAction(int n_x, int n_y, int n_w, int n_h):
        x(n_x), y(n_y), w(n_w), h(n_h) {}

    virtual QString toString () {return QString("plasma-block at %1 %2 %3 %4").arg(x).arg(y).arg(w).arg(h);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list) = 0;
    
    virtual QRectF boundingBox() {return QRectF(x, y, w, h);}
    virtual bool contains (const QPointF& point) {return boundingBox().contains(point);}
};

class KnotPlasmaBlockActionFactory
{
public:
    virtual ~KnotPlasmaBlockActionFactory ();
    virtual KnotPlasmaBlockAction* getAction (int x, int y, int w, int h) = 0;
};

class KnotPlasmaArrowAction : public KnotRendererBatch::KnotBatchAction
{
public:
    const int cx, cy, radius;
    enum Type{
        UP, DOWN, LEFT, RIGHT
    } type;

    KnotPlasmaArrowAction(int n_cx, int n_cy, int n_radius, Type n_type):
        cx(n_cx), cy(n_cy), radius(n_radius), type(n_type) {}

    virtual QString toString () {return QString("plasma-arrow at %1 %2 %3").arg(cx).arg(cy).arg(radius);}
    virtual void apply (KnotRendererBatch::PaintInterfaceData* paint_interface, const QList<QColor>& color_list) = 0;
    
    virtual QRectF boundingBox() {return QRectF(cx-radius, cy-radius, radius*2, radius*2);}
    virtual bool contains (const QPointF& point) {return boundingBox().contains(point);}
};

class KnotPlasmaArrowActionFactory
{
public:
    virtual ~KnotPlasmaArrowActionFactory ();
    virtual KnotPlasmaArrowAction* getAction (int cx, int cy, int radius, KnotPlasmaArrowAction::Type type) = 0;
};

struct GameHandlerFactories
{
    KnotPlasmaCircleActionFactory *circle_factory;
    KnotPlasmaBlockActionFactory *block_factory;
    KnotPlasmaRectActionFactory *rect_factory;
    KnotPlasmaArrowActionFactory *arrow_factory;
};

class DefaultGameHandler: public KnotRendererPlasma::GameHandler
{
public:
    virtual ~DefaultGameHandler ();
    DefaultGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
protected:
    bool containsByElements(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    bool containsByPreprocessedElements(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    void genericRemoveSpace(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    void getRealDimensionByBoundingBox (int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    const GameHandlerFactories m_factories;
};

class BlackBoxGameHandler: public DefaultGameHandler
{
public:
    virtual ~BlackBoxGameHandler ();
    BlackBoxGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class BridgesGameHandler: public DefaultGameHandler
{
public:
    virtual ~BridgesGameHandler ();
    BridgesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class CubeGameHandler: public DefaultGameHandler
{
public:
    virtual ~CubeGameHandler ();
    CubeGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class DominosaGameHandler: public DefaultGameHandler
{
public:
    virtual ~DominosaGameHandler ();
    DominosaGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class FifteenGameHandler: public DefaultGameHandler
{
public:
    virtual ~FifteenGameHandler ();
    FifteenGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class FillingGameHandler: public DefaultGameHandler
{
public:
    virtual ~FillingGameHandler ();
    FillingGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class FlipGameHandler: public DefaultGameHandler
{
public:
    virtual ~FlipGameHandler ();
    FlipGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class GalaxiesGameHandler: public DefaultGameHandler
{
public:
    virtual ~GalaxiesGameHandler ();
    GalaxiesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class InertiaGameHandler: public DefaultGameHandler
{
public:
    virtual ~InertiaGameHandler ();
    InertiaGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class LoopyGameHandler: public DefaultGameHandler
{
public:
    virtual ~LoopyGameHandler ();
    LoopyGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class MapGameHandler: public DefaultGameHandler
{
public:
    virtual ~MapGameHandler ();
    MapGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class MinesGameHandler: public DefaultGameHandler
{
public:
    virtual ~MinesGameHandler ();
    MinesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class PearlGameHandler: public DefaultGameHandler
{
public:
    virtual ~PearlGameHandler ();
    PearlGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class RangeGameHandler: public DefaultGameHandler
{
public:
    virtual ~RangeGameHandler ();
    RangeGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SameGameGameHandler: public DefaultGameHandler
{
public:
    virtual ~SameGameGameHandler ();
    SameGameGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SignpostGameHandler: public DefaultGameHandler
{
public:
    virtual ~SignpostGameHandler ();
    SignpostGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SinglesGameHandler: public DefaultGameHandler
{
public:
    virtual ~SinglesGameHandler ();
    SinglesGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SixteenGameHandler: public DefaultGameHandler
{
public:
    virtual ~SixteenGameHandler ();
    SixteenGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SlantGameHandler: public DefaultGameHandler
{
public:
    virtual ~SlantGameHandler ();
    SlantGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SlideGameHandler: public DefaultGameHandler
{
public:
    virtual ~SlideGameHandler ();
    SlideGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class SoloGameHandler: public DefaultGameHandler
{
public:
    virtual ~SoloGameHandler ();
    SoloGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class TowersGameHandler: public DefaultGameHandler
{
public:
    virtual ~TowersGameHandler ();
    TowersGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class UndeadGameHandler: public DefaultGameHandler
{
public:
    virtual ~UndeadGameHandler ();
    UndeadGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class UnequalGameHandler: public DefaultGameHandler
{
public:
    virtual ~UnequalGameHandler ();
    UnequalGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class UnrulyGameHandler: public DefaultGameHandler
{
public:
    virtual ~UnrulyGameHandler ();
    UnrulyGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

class UntangleGameHandler: public DefaultGameHandler
{
public:
    virtual ~UntangleGameHandler ();
    UntangleGameHandler(const GameHandlerFactories& factories);
    virtual void preprocessBatch(QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual void getRealDimension(int& x, int& y, int& ox, int& oy, QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch);
    virtual bool contains(const QPointF& point, const QList<std::shared_ptr<KnotRendererBatch::KnotBatchAction>>& batch, const QSizeF& size);
    virtual void free();
};

#endif
